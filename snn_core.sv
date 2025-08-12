`timescale 1ns/1ps
`default_nettype none

module snn_core #(
    parameter int F = 48,
    parameter int N = 96,
    parameter int Q = 14,
    // alpha = exp(-dt/tau) with dt=0.002, tau=0.035  → 0.944459... ≈ 15474 (Q1.14)
    parameter int ALPHA_Q14 = 15474
)(
    input  logic                   clk,
    input  logic                   rstn,

    input  logic  [F-1:0]          event_vec,   // 0/1 events for this time step
    output logic  [N-1:0]          spikes_vec,

    // ===== STDP ports =====
    input  logic                   stdp_enable,
    input  logic [F-1:0]           stdp_pre_bits,
    input  logic [N-1:0]           stdp_post_bits,

    input  logic signed [15:0]     stdp_eta,
    input  logic        [7:0]      stdp_eta_shift,
    input  logic signed [15:0]     stdp_lambda_x,
    input  logic signed [15:0]     stdp_lambda_y,
    input  logic signed [15:0]     stdp_b_pre,
    input  logic signed [15:0]     stdp_b_post,
    input  logic signed [15:0]     stdp_wmin,
    input  logic signed [15:0]     stdp_wmax,
    input  logic                   stdp_enable_pre,
    input  logic                   stdp_enable_post,

    output logic                   stdp_w_we,
    output logic [$clog2(F*N)-1:0] stdp_w_addr,
    output logic signed [15:0]     stdp_w_wdata,
    input  logic signed [15:0]     stdp_w_rdata
);

    localparam int AW = $clog2(F*N);

    // ------------------------
    // ROM/RAM (TB가 public 배열에 $readmemh 후 복사)
    // ------------------------
    /* verilator public_flat */ logic signed [15:0] weights_rom [0:(F*N)-1]; // Q1.14 signed
    /* verilator public_flat */ logic signed [15:0] vth_rom     [0:N-1];     // Q1.14 signed
    /* verilator public_flat */ logic signed [15:0] weights_ram [0:(F*N)-1]; // Q1.14 signed (STDP write-back 대상)

    // ------------------------
    // State (Q1.14 & refrac)
    // ------------------------
    logic signed [15:0] V_q14   [0:N-1];  // membrane potential (Q1.14)
    int                 refrac  [0:N-1];  // refractory counter (cycles)

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
        // 기본값 (래치 방지)
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
                logic signed [15:0] w16_local = 16'sd0;
                logic signed [31:0] w32_local = 32'sd0;
                if (event_vec[f]) begin
                    // inference는 ROM 사용 (학습은 RAM에 write-back)
                    w16_local = weights_rom[f*N + n1];         // Q1.14
                    w32_local = {{16{w16_local[15]}}, w16_local}; // sign-extend 16→32
                end
                acc32[n1] += w32_local; // 32비트 누적
            end
        end

        // next V (Q1.14) with **signed rounding** on leak path
        for (int n2 = 0; n2 < N; n2++) begin
            logic signed [31:0] sum32_local = 32'sd0;
            // signed round-to-nearest for right shift by Q
            logic signed [31:0] bias = (leak32[n2] >= 0)
                                     ? (32'sd1 <<< (Q-1))
                                     : -(32'sd1 <<< (Q-1));
            sum32_local = ((leak32[n2] + bias) >>> Q) + acc32[n2];

            // saturate to 16-bit signed
            if (sum32_local > 32'sd32767)       V_next[n2] = 16'sd32767;
            else if (sum32_local < -32'sd32768) V_next[n2] = -16'sd32768;
            else                                 V_next[n2] = sum32_local[15:0];
        end
    end

    // ------------------------
    // Sequential update per step
    // ------------------------
    always_ff @(posedge clk or negedge rstn) begin
        if (!rstn) begin
            for (int n = 0; n < N; n++) begin
                V_q14[n]      = '0;
                refrac[n]     = 0;
                spikes_vec[n] = 1'b0;
            end
        end else begin
            for (int n = 0; n < N; n++) begin
                // Spike decision (>=) & refractory gating
                if (active[n] && ($signed(V_next[n]) >= $signed(vth_rom[n]))) begin
                    spikes_vec[n] = 1'b1;
                    V_q14[n]      = 16'sd0; // v_reset = 0 (Q1.14)
                    refrac[n]     = 2;      // ~0.004s at dt=0.002s
                end else begin
                    spikes_vec[n] = 1'b0;
                    V_q14[n]      = V_next[n];
                    refrac[n]     = (refrac[n] > 0) ? (refrac[n] - 1) : 0;
                end
            end
        end
    end

    // ========================
    // STDP write-back to RAM
    // ========================
    always_ff @(posedge clk) begin
        if (stdp_w_we) begin
            weights_ram[stdp_w_addr] <= stdp_w_wdata;
        end
    end

endmodule

`default_nettype wire
