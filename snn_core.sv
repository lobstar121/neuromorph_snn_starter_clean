// snn_core.sv — Q1.14 LIF SNN core (F inputs × N neurons)
// - Weights/Vth: Q1.14 signed, testbench가 $readmemh로 로드 후 hier. assign
// - One time step per posedge clk
// - Weight ROM addressing: addr = f*N + n  (feature-major, row-major)

`timescale 1ns/1ps
`default_nettype none

module snn_core #(
    parameter int F = 48,
    parameter int N = 96,
    parameter int Q = 14,
    // alpha ≈ exp(-dt/tau) with dt=0.002, tau=0.035  → 0.944459.. ≈ 15474 (Q1.14)
    parameter int ALPHA_Q14 = 15474
)(
    input  logic                 clk,
    input  logic                 rstn,

    // this-timestep input events (0/1)
    input  logic  [F-1:0]        event_vec,
    // this-timestep output spikes (0/1)
    output logic  [N-1:0]        spikes_vec,

    // ===================== STDP skeleton ports =====================
    // (지금은 기능 OFF; tie-off. 이후 stdp_q14와 배선)
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

    // 가중치 write-back 인터페이스(순차형 업데이트용)
    output logic                  stdp_w_we,
    output logic [((F*N)<=1)?1:$clog2(F*N)-1:0] stdp_w_addr,
    output logic signed [15:0]    stdp_w_wdata,
    // ★ 현재 가중치 read 데이터(향후 stdp_q14가 사용)
    input  logic signed [15:0]    stdp_w_rdata
);
    // 주소폭 localparam (표현식과 동일하게 유지)
    localparam int AW = (F*N <= 1) ? 1 : $clog2(F*N);

    // ------------------------
    // ROMs loaded by testbench
    // ------------------------
    // testbench에서 dut.weights_rom[i] = ... 식으로 채움
    logic signed [15:0] weights_rom [0:(F*N)-1]; // Q1.14
    logic signed [15:0] vth_rom     [0:N-1];     // Q1.14

    // ------------------------
    // State (Q1.14 & refrac)
    // ------------------------
    logic signed [15:0] V_q14 [0:N-1];  // membrane potential (Q1.14)
    int                 refrac[0:N-1];  // refractory counter (cycles)

    // ------------------------
    // Combinational signals
    // ------------------------
    logic               active   [0:N-1];
    logic signed [31:0] leak32   [0:N-1]; // alpha*V (Q1.14 * Q1.14 → Q2.28 in 32b)
    logic signed [31:0] acc32    [0:N-1]; // input sum (Q1.14 widened to 32b)
    logic signed [15:0] V_next   [0:N-1];

    // ------------------------
    // Combinational datapath
    // ------------------------
    always_comb begin
        // 기본값(래치 방지)
        for (int i = 0; i < N; i++) begin
            active[i] = 1'b0;
            leak32[i] = 32'sd0;
            acc32[i]  = 32'sd0;
            V_next[i] = 16'sd0;
        end

        // refractory → active
        for (int n0 = 0; n0 < N; n0++) begin
            active[n0] = (refrac[n0] <= 0);
        end

        // leak & input accumulation
        for (int n1 = 0; n1 < N; n1++) begin
            // leak: (ALPHA_Q14 * V_q14[n])  (Q2.28 in 32b)
            leak32[n1] = $signed(ALPHA_Q14) * $signed(V_q14[n1]);

            // input dot: sum_f (event[f] ? weight[f*N + n] : 0)
            acc32[n1]  = 32'sd0;
            for (int f = 0; f < F; f++) begin
                // 모든 경로에서 로컬 값 갖도록 기본값
                logic signed [15:0] w16_local = 16'sd0;
                logic signed [31:0] w32_local = 32'sd0;
                if (event_vec[f]) begin
                    w16_local = weights_rom[f*N + n1];           // Q1.14
                    w32_local = {{16{w16_local[15]}}, w16_local}; // sign-extend 16→32
                end
                acc32[n1] += w32_local; // 32비트 누적
            end
        end

        // next V (Q1.14) with **signed rounding** on leak path
        for (int n2 = 0; n2 < N; n2++) begin
            // signed round-to-nearest for >>> Q
            logic signed [31:0] bias = (leak32[n2] >= 0)
                                     ? (32'sd1 <<< (Q-1))
                                     : -(32'sd1 <<< (Q-1));
            logic signed [31:0] sum32_local;
            sum32_local = ((leak32[n2] + bias) >>> Q) + acc32[n2];

            // saturate to 16-bit signed
            if (sum32_local > 32'sd32767)       V_next[n2] = 16'sd32767;
            else if (sum32_local < -32'sd32768) V_next[n2] = -16'sd32768;
            else                                 V_next[n2] = sum32_local[15:0];
        end
    end

    // ------------------------
    // Sequential update per step
    // Verilator 제약: for 루프 내 배열에는 블로킹 '=' 사용
    // ------------------------
    always_ff @(posedge clk or negedge rstn) begin
        if (!rstn) begin
            for (int n = 0; n < N; n++) begin
                V_q14[n]      = '0;    // '=' (blocking)
                refrac[n]     = 0;
                spikes_vec[n] = 1'b0;
            end
        end else begin
            for (int n = 0; n < N; n++) begin
                if (active[n] && ($signed(V_next[n]) >= $signed(vth_rom[n]))) begin
                    spikes_vec[n] = 1'b1;
                    V_q14[n]      = 16'sd0;     // v_reset = 0 (Q1.14)
                    refrac[n]     = 2;          // 예: 2 사이클
                end else begin
                    spikes_vec[n] = 1'b0;
                    V_q14[n]      = V_next[n];
                    refrac[n]     = (refrac[n] > 0) ? (refrac[n] - 1) : 0;
                end
            end
        end
    end

    // ------------------------
    // STDP tie-offs (기능 OFF)
    // ------------------------
    // 현재는 write-back 비활성. 이후 stdp_q14 인스턴스 배선 예정.
    always_comb begin
        // 출력 0
        stdp_w_we    = 1'b0;
        stdp_w_addr  = '0;
        stdp_w_wdata = '0;
        // read 데이터는 상위에서 연결해 사용 (필요시 아래 주석 해제)
        // assign은 comb 블록 밖에서도 가능하지만, 여기선 사용 안함.
        // stdp_w_rdata는 입력이므로 별도 처리 없음.
        // 향후:
        // assign stdp_w_rdata = weights_rom[stdp_w_addr];
    end

endmodule

`default_nettype wire
