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
    input  logic  [F-1:0]        event_vec,
    output logic  [N-1:0]        spikes_vec,

    // ---- STDP skeleton ports (아직 OFF) ----
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
    output logic                  stdp_w_we,
    output logic [((F*N)<=1)?1:$clog2(F*N)-1:0] stdp_w_addr,
    output logic signed [15:0]    stdp_w_wdata,
    input  logic signed [15:0]    stdp_w_rdata
);
    localparam int AW = (F*N <= 1) ? 1 : $clog2(F*N);

    // TB가 hier로 채우는 ROM
    logic signed [15:0] weights_rom [0:(F*N)-1];
    logic signed [15:0] vth_rom     [0:N-1];

    // 상태
    logic signed [15:0] V_q14 [0:N-1];
    int                 refrac[0:N-1];

    // 콤비
    logic               active   [0:N-1];
    logic signed [31:0] leak32   [0:N-1];
    logic signed [31:0] acc32    [0:N-1];
    logic signed [15:0] V_next   [0:N-1];

    // 누수/입력 합산
    always_comb begin
        for (int i = 0; i < N; i++) begin
            active[i] = 1'b0;
            leak32[i] = 32'sd0;
            acc32[i]  = 32'sd0;
            V_next[i] = 16'sd0;
        end

        for (int n0 = 0; n0 < N; n0++) begin
            active[n0] = (refrac[n0] <= 0);
        end

        for (int n1 = 0; n1 < N; n1++) begin
            leak32[n1] = $signed(ALPHA_Q14) * $signed(V_q14[n1]);

            acc32[n1]  = 32'sd0;
            for (int f = 0; f < F; f++) begin
                logic signed [15:0] w16_local = 16'sd0;
                logic signed [31:0] w32_local = 32'sd0;
                if (event_vec[f]) begin
                    w16_local = weights_rom[f*N + n1];
                    w32_local = {{16{w16_local[15]}}, w16_local};
                end
                acc32[n1] += w32_local;
            end
        end

        for (int n2 = 0; n2 < N; n2++) begin
            // Verilator 메모: for 루프 안의 배열 갱신은 블로킹 '=' 사용
            logic signed [31:0] bias =
                (leak32[n2] >= 0) ? (32'sd1 <<< (Q-1)) : -(32'sd1 <<< (Q-1));
            logic signed [31:0] sum32_local;
            sum32_local = ((leak32[n2] + bias) >>> Q) + acc32[n2];

            if (sum32_local > 32'sd32767)       V_next[n2] = 16'sd32767;
            else if (sum32_local < -32'sd32768) V_next[n2] = -16'sd32768;
            else                                 V_next[n2] = sum32_local[15:0];
        end
    end

    // 한 스텝 업데이트 (for 루프 안 배열은 '=')
    always_ff @(posedge clk or negedge rstn) begin
        if (!rstn) begin
            for (int n = 0; n < N; n++) begin
                V_q14[n]      = '0;
                refrac[n]     = 0;
                spikes_vec[n] = 1'b0;
            end
        end else begin
            for (int n = 0; n < N; n++) begin
                if ((refrac[n] <= 0) && ($signed(V_next[n]) >= $signed(vth_rom[n]))) begin
                    spikes_vec[n] = 1'b1;
                    V_q14[n]      = 16'sd0;
                    refrac[n]     = 2;
                end else begin
                    spikes_vec[n] = 1'b0;
                    V_q14[n]      = V_next[n];
                    refrac[n]     = (refrac[n] > 0) ? (refrac[n] - 1) : 0;
                end
            end
        end
    end

    // STDP tie-off (아직 미사용)
    always_comb begin
        stdp_w_we    = 1'b0;
        stdp_w_addr  = '0;
        stdp_w_wdata = '0;
    end

endmodule

`default_nettype wire
