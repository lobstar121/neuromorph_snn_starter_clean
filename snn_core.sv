// snn_core.sv — Q1.14 LIF SNN core + internal STDP write-back (1 weight/clk)

`timescale 1ns/1ps
`default_nettype none

module snn_core #(
    parameter int F = 48,
    parameter int N = 96,
    parameter int Q = 14,
    // alpha = exp(-dt/tau) with dt=0.002, tau=0.035 → 0.944459.. ≈ 15474 (Q1.14)
    parameter int ALPHA_Q14 = 15474
)(
    input  logic                 clk,
    input  logic                 rstn,

    input  logic  [F-1:0]        event_vec,     // 0/1 inputs at this step
    output logic  [N-1:0]        spikes_vec,    // spikes at this step

    // STDP enable (sequential learner runs when 1)
    input  logic                 stdp_enable
);

    // =========================================================
    // Memories (TB가 초기값을 여기에 채운다)
    //   - weights_rom/vth_rom : TB에서 $readmemh → 여기에 복사
    //   - weights_ram         : 동작/학습용 RAM (누적 및 STDP write-back)
    // =========================================================
    logic signed [15:0] weights_rom [0:(F*N)-1];
    logic signed [15:0] vth_rom     [0:N-1];
    logic signed [15:0] weights_ram [0:(F*N)-1];

    // =========================================================
    // 상태
    // =========================================================
    logic signed [15:0] V_q14   [0:N-1];
    int                 refrac  [0:N-1];

    // =========================================================
    // 조합 경로
    // =========================================================
    logic               active   [0:N-1];
    logic signed [31:0] leak32   [0:N-1];
    logic signed [31:0] acc32    [0:N-1];
    logic signed [15:0] V_next   [0:N-1];

    always_comb begin
        for (int n = 0; n < N; n++) begin
            active[n] = (refrac[n] <= 0);

            // leak: (ALPHA_Q14 * V_q14) → Q2.28 in 32b
            leak32[n] = $signed(ALPHA_Q14) * $signed(V_q14[n]);

            // input accumulation (from weights_ram)
            acc32[n]  = 32'sd0;
            for (int f = 0; f < F; f++) begin
                logic signed [15:0] w16 = 16'sd0;
                if (event_vec[f]) begin
                    w16 = weights_ram[f*N + n];
                end
                acc32[n] += {{16{w16[15]}}, w16}; // sign-extend 16→32
            end
        end

        // next V with signed rounding on leak
        for (int n2 = 0; n2 < N; n2++) begin
            logic signed [31:0] bias = (leak32[n2] >= 0)
                                     ? (32'sd1 <<< (Q-1))
                                     : -(32'sd1 <<< (Q-1));
            logic signed [31:0] sum32 = ((leak32[n2] + bias) >>> Q) + acc32[n2];

            if (sum32 > 32'sd32767)       V_next[n2] = 16'sd32767;
            else if (sum32 < -32'sd32768) V_next[n2] = -16'sd32768;
            else                          V_next[n2] = sum32[15:0];
        end
    end

    // =========================================================
    // 시퀀셜 업데이트 (한 타임스텝)
    //   - 리셋 시 ROM → RAM 복사
    // =========================================================
    always_ff @(posedge clk or negedge rstn) begin
        if (!rstn) begin
            for (int n = 0; n < N; n++) begin
                V_q14[n]      <= '0;
                refrac[n]     <= 0;
                spikes_vec[n] <= 1'b0;
            end
            for (int i = 0; i < F*N; i++) begin
                weights_ram[i] <= weights_rom[i];
            end
        end else begin
            for (int n = 0; n < N; n++) begin
                if ((refrac[n] <= 0) && ($signed(V_next[n]) >= $signed(vth_rom[n]))) begin
                    spikes_vec[n] <= 1'b1;
                    V_q14[n]      <= 16'sd0;   // v_reset
                    refrac[n]     <= 2;        // dt=0.002 → ~0.004s
                end else begin
                    spikes_vec[n] <= 1'b0;
                    V_q14[n]      <= V_next[n];
                    refrac[n]     <= (refrac[n] > 0) ? (refrac[n]-1) : 0;
                end
            end
        end
    end

    // =========================================================
    // 내부 STDP (순차형, 클럭당 1가중치 업데이트)
    // =========================================================
    // SW 기준값과 동일 (원하면 TB 포트/plusargs로 뺄 수 있음)
    localparam logic signed [15:0] P_ETA        = 16'sd8;
    localparam logic        [7:0]  P_ETA_SHIFT  = 8'd12;
    localparam logic signed [15:0] P_LAMBDA_X   = 16'sd15565;
    localparam logic signed [15:0] P_LAMBDA_Y   = 16'sd15565;
    localparam logic signed [15:0] P_B_PRE      = 16'sd1024;
    localparam logic signed [15:0] P_B_POST     = 16'sd1024;
    localparam logic signed [15:0] P_WMIN       = -16'sd16384;
    localparam logic signed [15:0] P_WMAX       =  16'sd16384;
    localparam logic               P_EN_PRE     = 1'b1;
    localparam logic               P_EN_POST    = 1'b1;

    localparam int AW = $clog2(F*N);
    logic                  stdp_we;
    logic [AW-1:0]         stdp_addr;
    logic signed [15:0]    stdp_wdata;
    logic signed [15:0]    stdp_rdata;

    // write-back
    always_ff @(posedge clk) begin
        if (stdp_enable && stdp_we) begin
            weights_ram[stdp_addr] <= stdp_wdata;
        end
    end
    assign stdp_rdata = weights_ram[stdp_addr];

    // learner
    stdp_q14 #(.F(F), .N(N), .Q(Q)) u_stdp (
        .clk         (clk),
        .rst_n       (rstn),
        .enable      (stdp_enable),

        .pre_bits    (event_vec),
        .post_bits   (spikes_vec),

        .eta         (P_ETA),
        .eta_shift   (P_ETA_SHIFT),
        .lambda_x    (P_LAMBDA_X),
        .lambda_y    (P_LAMBDA_Y),
        .b_pre       (P_B_PRE),
        .b_post      (P_B_POST),
        .wmin        (P_WMIN),
        .wmax        (P_WMAX),
        .enable_pre  (P_EN_PRE),
        .enable_post (P_EN_POST),

        .w_we        (stdp_we),
        .w_addr      (stdp_addr),
        .w_wdata     (stdp_wdata),
        .w_rdata     (stdp_rdata)
    );

endmodule

`default_nettype wire
