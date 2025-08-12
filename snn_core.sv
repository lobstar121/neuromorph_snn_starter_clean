// snn_core.sv — Q1.14 LIF SNN core + STDP write-back (1 weight/clk)

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

    // step input / output
    input  logic  [F-1:0]        event_vec,
    output logic  [N-1:0]        spikes_vec,

    // ===== STDP ports =====
    input  logic                 stdp_enable,
    input  logic  [F-1:0]        stdp_pre_bits,
    input  logic  [N-1:0]        stdp_post_bits,

    input  logic  signed [15:0]  stdp_eta,
    input  logic         [7:0]   stdp_eta_shift,
    input  logic  signed [15:0]  stdp_lambda_x,
    input  logic  signed [15:0]  stdp_lambda_y,
    input  logic  signed [15:0]  stdp_b_pre,
    input  logic  signed [15:0]  stdp_b_post,
    input  logic  signed [15:0]  stdp_wmin,
    input  logic  signed [15:0]  stdp_wmax,
    input  logic                 stdp_enable_pre,
    input  logic                 stdp_enable_post,

    // readback (for TB dump of learned weights)
    input  logic  [$clog2(F*N)-1:0]  rb_addr,
    output logic  signed [15:0]      rb_data
);
    localparam int AW = $clog2(F*N);

    // ---- Memories (TB가 weights_rom/vth_rom을 채움) ----
    logic signed [15:0] weights_rom [0:(F*N)-1];  // init-only
    logic signed [15:0] vth_rom     [0:N-1];
    logic signed [15:0] weights_ram [0:(F*N)-1];  // forward 사용 + STDP가 갱신

    // ---- State ----
    logic signed [15:0] V_q14 [0:N-1];
    int                 refrac[0:N-1];

    // ---- Combinational ----
    logic               active [0:N-1];
    logic signed [31:0] leak32 [0:N-1];
    logic signed [31:0] acc32  [0:N-1];
    logic signed [15:0] V_next [0:N-1];

    // weight addr helper
    function automatic [AW-1:0] fn_addr(input int f, input int n);
        logic [31:0] tmp;
        begin
            tmp     = (32'(f) * 32'(N)) + 32'(n);
            fn_addr = AW'(tmp);
        end
    endfunction

    // forward (combinational)
    always_comb begin
        for (int n = 0; n < N; n++) begin
            active[n] = (refrac[n] <= 0);
            leak32[n] = $signed(ALPHA_Q14) * $signed(V_q14[n]);
            acc32[n]  = 32'sd0;
            for (int f = 0; f < F; f++) begin
                logic signed [15:0] w16 = 16'sd0;
                if (event_vec[f]) w16 = weights_ram[fn_addr(f,n)];
                acc32[n] += {{16{w16[15]}}, w16};
            end
        end
        for (int n2 = 0; n2 < N; n2++) begin
            logic signed [31:0] bias = (leak32[n2] >= 0)
                                     ? (32'sd1 <<< (Q-1))
                                     : -(32'sd1 <<< (Q-1));
            logic signed [31:0] sum32 = ((leak32[n2] + bias) >>> Q) + acc32[n2];
            if      (sum32 >  32'sd32767)  V_next[n2] = 16'sd32767;
            else if (sum32 < -32'sd32768)  V_next[n2] = -16'sd32768;
            else                           V_next[n2] = sum32[15:0];
        end
    end

    // readback
    assign rb_data = weights_ram[rb_addr];

    // ---- STDP instance ----
    logic                 w_we;
    logic [AW-1:0]        w_addr;
    logic signed [15:0]   w_wdata;
    wire  signed [15:0]   w_rdata = weights_ram[w_addr];

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

        .w_we       (w_we),
        .w_addr     (w_addr),
        .w_wdata    (w_wdata),
        .w_rdata    (w_rdata)
    );

    // ---- Seq update ----
    always_ff @(posedge clk or negedge rstn) begin
        if (!rstn) begin
            // init V/refrac
            for (int n = 0; n < N; n++) begin
                V_q14[n]  = '0;
                refrac[n] = 0;
            end
            // copy ROM->RAM (sim-only)
            for (int i = 0; i < F*N; i++) begin
                weights_ram[i] = weights_rom[i];
            end
            spikes_vec <= '0;
        end else begin
            // STDP write-back (one weight/clk)
            if (w_we) weights_ram[w_addr] <= w_wdata;

            for (int n = 0; n < N; n++) begin
                if (active[n] && ($signed(V_next[n]) >= $signed(vth_rom[n]))) begin
                    spikes_vec[n] <= 1'b1;
                    V_q14[n]      <= 16'sd0;
                    refrac[n]     <= 2;
                end else begin
                    spikes_vec[n] <= 1'b0;
                    V_q14[n]      <= V_next[n];
                    refrac[n]     <= (refrac[n] > 0) ? (refrac[n]-1) : 0;
                end
            end
        end
    end
endmodule

`default_nettype wire
