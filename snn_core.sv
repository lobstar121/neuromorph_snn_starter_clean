`timescale 1ns/1ps
`default_nettype none

module snn_core #(
    parameter int F = 48,
    parameter int N = 96,
    parameter int Q = 14,
    parameter int ALPHA_Q14 = 15474
)(
    input  logic                 clk,
    input  logic                 rstn,

    input  logic  [F-1:0]        event_vec,    // 0/1 events
    output logic  [N-1:0]        spikes_vec,

    // ==== STDP control (from TB) ====
    input  logic                  stdp_enable,
    input  logic [F-1:0]          stdp_pre_bits,
    input  logic [N-1:0]          stdp_post_bits,

    input  logic signed [15:0]    stdp_eta,
    input  logic        [7:0]     stdp_eta_shift,
    input  logic signed [15:0]    stdp_lambda_x,
    input  logic signed [15:0]    stdp_lambda_y,
    input  logic signed [15:0]    stdp_b_pre,
    input  logic signed [15:0]    stdp_b_post,
    input  logic signed [15:0]    stdp_wmin,
    input  logic signed [15:0]    stdp_wmax,
    input  logic                  stdp_enable_pre,
    input  logic                  stdp_enable_post,

    // readback
    input  logic [$clog2(F*N)-1:0] rb_addr,
    output logic signed [15:0]     rb_data
);

    localparam int AW = $clog2(F*N);

    // ROMs loaded by TB
    logic signed [15:0] weights_rom [0:(F*N)-1];
    logic signed [15:0] vth_rom     [0:N-1];

    // Working RAM for weights (STDP writes here)
    logic signed [15:0] weights_ram [0:(F*N)-1];

    // State
    logic signed [15:0] V_q14 [0:N-1];
    int                 refrac[0:N-1];

    // datapath temps
    logic               active   [0:N-1];
    logic signed [31:0] leak32   [0:N-1];
    logic signed [31:0] acc32    [0:N-1];
    logic signed [15:0] V_next   [0:N-1];

    // ----- readback -----
    assign rb_data = weights_ram[rb_addr];

    // ----- STDP instance -----
    logic                  w_we_i;
    logic [AW-1:0]         w_addr_i;
    logic signed [15:0]    w_wdata_i;
    logic signed [15:0]    w_rdata_i;

    // combinational read for STDP
    assign w_rdata_i = weights_ram[w_addr_i];

    stdp_q14 #(.F(F), .N(N), .Q(Q)) u_stdp (
        .clk        (clk),
        .rst_n      (rstn),
        .enable     (stdp_enable),

        .pre_bits   (stdp_pre_bits),
        .post_bits  (stdp_post_bits),

        .eta        (stdp_eta),
        .eta_shift  (stdp_eta_shift),
        .lambda_x   (stdp_lambda_x),
        .lambda_y   (stdp_lambda_y),
        .b_pre      (stdp_b_pre),
        .b_post     (stdp_b_post),
        .wmin       (stdp_wmin),
        .wmax       (stdp_wmax),
        .enable_pre (stdp_enable_pre),
        .enable_post(stdp_enable_post),

        .w_we       (w_we_i),
        .w_addr     (w_addr_i),
        .w_wdata    (w_wdata_i),
        .w_rdata    (w_rdata_i)
    );

    // ----- combinational datapath -----
    always_comb begin
        // 기본값으로 모두 초기화 → 래치 방지
        for (int i = 0; i < N; i++) begin
            active[i] = 1'b0;
            leak32[i] = 32'sd0;
            acc32[i]  = 32'sd0;
            V_next[i] = 16'sd0;
        end

        // active & leak
        for (int n = 0; n < N; n++) begin
            active[n] = (refrac[n] <= 0);
            leak32[n] = $signed(ALPHA_Q14) * $signed(V_q14[n]); // Q1.14*Q1.14
        end

        // input accumulation (모든 경로에서 w16/w32 기본값 부여 → 래치 방지)
        for (int n = 0; n < N; n++) begin
            for (int f = 0; f < F; f++) begin
                logic signed [15:0] w16 = 16'sd0;
                logic signed [31:0] w32 = 32'sd0;
                if (event_vec[f]) begin
                    w16 = weights_ram[f*N + n];
                    w32 = {{16{w16[15]}}, w16};
                end
                acc32[n] += w32;
            end
        end

        // next V with signed rounding shift (bias/sum32도 기본값 경로 존재)
        for (int n = 0; n < N; n++) begin
            logic signed [31:0] bias  = 32'sd0;
            logic signed [31:0] sum32 = 32'sd0;
            bias  = (leak32[n] >= 0) ? (32'sd1 <<< (Q-1))
                                     : -(32'sd1 <<< (Q-1));
            sum32 = ((leak32[n] + bias) >>> Q) + acc32[n];
            if (sum32 > 32'sd32767)       V_next[n] = 16'sd32767;
            else if (sum32 < -32'sd32768) V_next[n] = -16'sd32768;
            else                           V_next[n] = sum32[15:0];
        end
    end

    // ----- sequential update -----
    always_ff @(posedge clk or negedge rstn) begin
        if (!rstn) begin
            // init states
            for (int n = 0; n < N; n++) begin
                V_q14[n]      = '0;      // 블로킹(Verilator 제약)
                refrac[n]     = 0;
                spikes_vec[n] = 1'b0;
            end
            // init RAM from ROM (TB가 ROM을 먼저 채움)
            for (int i = 0; i < F*N; i++) begin
                weights_ram[i] = weights_rom[i]; // 블로킹
            end
        end else begin
            // STDP write-back (1 write/clk)
            if (w_we_i) begin
                weights_ram[w_addr_i] = w_wdata_i; // 블로킹
            end

            // membrane & spikes
            for (int n = 0; n < N; n++) begin
                if (active[n] && ($signed(V_next[n]) >= $signed(vth_rom[n]))) begin
                    spikes_vec[n] = 1'b1;
                    V_q14[n]      = 16'sd0;      // reset
                    refrac[n]     = 2;           // refractory
                end else begin
                    spikes_vec[n] = 1'b0;
                    V_q14[n]      = V_next[n];
                    refrac[n]     = (refrac[n] > 0) ? (refrac[n]-1) : 0;
                end
            end
        end
    end
endmodule

`default_nettype wire
