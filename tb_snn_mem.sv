`timescale 1ns/1ps
`default_nettype none

module tb_snn_mem;
    // ---- DUT 파라미터 ----
    localparam int F = 48;
    localparam int N = 96;
    localparam int Q = 14;
    parameter  int ALPHA_Q14 = 15474;

    // ---- 클럭/리셋 ----
    logic clk  = 0;
    logic rstn = 0;

    // 1ns 토글 클럭 (Verilator 5.x는 --timing 필요)
    always #1 clk = ~clk;

    // ---- 입출력 레지스터/버퍼 ----
    logic [F-1:0] event_vec_reg;
    logic [F-1:0] event_next;
    logic [N-1:0] spikes_vec;

    // ---- plusargs & 파일 핸들 ----
    string whex, vthx, evhex, outcsv;
    integer ofile;
    int T;

    // ---- TB에서 로드하는 메모리들 ----
    logic [F-1:0]        events_mem  [0:65535];      // stimulus (packed bits)
    logic signed [15:0]  weights_mem [0:(F*N)-1];    // Q1.14
    logic signed [15:0]  vth_mem     [0:N-1];        // Q1.14

    // ---- STDP 스켈레톤용 더미 신호 (지금은 OFF) ----
    localparam int AW = $clog2(F*N);
    logic                  stdp_w_we;
    logic [AW-1:0]         stdp_w_addr;
    logic signed [15:0]    stdp_w_wdata;

    // ---- DUT ----
    snn_core #(
        .F(F), .N(N), .Q(Q), .ALPHA_Q14(ALPHA_Q14)
    ) dut (
        .clk        (clk),
        .rstn       (rstn),
        .event_vec  (event_vec_reg),
        .spikes_vec (spikes_vec),

        // ===== STDP skeleton: 모두 비활성 (tie-off) =====
        .stdp_enable      (1'b0),
        .stdp_pre_bits    ('0),
        .stdp_post_bits   ('0),
        .stdp_eta         (16'sd0),
        .stdp_eta_shift   (8'd0),
        .stdp_lambda_x    (16'sd0),
        .stdp_lambda_y    (16'sd0),
        .stdp_b_pre       (16'sd0),
        .stdp_b_post      (16'sd0),
        .stdp_wmin        (16'sd0),
        .stdp_wmax        (16'sd0),
        .stdp_enable_pre  (1'b0),
        .stdp_enable_post (1'b0),

        // write-back 인터페이스 (현재 미사용)
        .stdp_w_we        (stdp_w_we),
        .stdp_w_addr      (stdp_w_addr),
        .stdp_w_wdata     (stdp_w_wdata),

        // 🔧 반드시 연결: 입력 핀 누락(PINMISSING) 방지
        .stdp_w_rdata     (16'sd0)
    );

    // ------------------------
    // 유틸 태스크들
    // ------------------------
    task load_plusargs();
        if (!$value$plusargs("WHEX=%s", whex))   whex   = "artifacts/weights.hex";
        if (!$value$plusargs("VTH=%s",  vthx))   vthx   = "artifacts/vth.hex";
        if (!$value$plusargs("EVHEX=%s", evhex)) evhex  = "artifacts/events_ref.mem";
        if (!$value$plusargs("OUT=%s",  outcsv)) outcsv = "artifacts/spikes_hw.csv";
        if (!$value$plusargs("T=%d",    T))      T      = 76;
        $display("[TB] WHEX=%s  VTH=%s  EVHEX=%s  OUT=%s  T=%0d", whex, vthx, evhex, outcsv, T);
    endtask

    task load_mems();
        $display("[TB] loading %s", whex);
        $readmemh(whex, weights_mem);
        $display("[TB] loading %s", vthx);
        $readmemh(vthx, vth_mem);
        $display("[TB] loading %s", evhex);
        $readmemh(evhex, events_mem);

        // DUT 내부 ROM으로 복사 (TB → 공개 배열)
        for (int i = 0; i < F*N; i++) dut.weights_rom[i] = weights_mem[i];
        for (int i = 0; i < N;   i++) dut.vth_rom[i]     = vth_mem[i];
    endtask

    task dump_spike_row_to_csv();
        for (int n = 0; n < N; n++) begin
            $fwrite(ofile, "%0d", spikes_vec[n] ? 1 : 0);
            if (n != N-1) $fwrite(ofile, ",");
        end
        $fwrite(ofile, "\n");
    endtask

    // ------------------------
    // 메인 시퀀스
    // ------------------------
    initial begin
        load_plusargs();
        load_mems();

        ofile = $fopen(outcsv, "w");
        if (ofile == 0) begin
            $display("[TB][ERROR] cannot open %s for write", outcsv);
            $finish;
        end

        // 초기 값
        event_vec_reg = '0;
        event_next    = '0;

        // 리셋
        repeat (4) @(posedge clk);
        rstn = 1;
        @(posedge clk);

        // Warm-up 1 tick: t=0 이벤트 준비 후 다음 엣지에서 적재
        event_next = events_mem[0];
        @(posedge clk);
        event_vec_reg = event_next;   // 블로킹 '='

        // 본 루프
        for (int t = 0; t < T; t++) begin
            // 다음 자극 준비
            if (t+1 < T) event_next = events_mem[t+1];
            else         event_next = '0;

            @(posedge clk);
            // 이 시점에서 spikes_vec는 직전 사이클 입력의 결과 → t번째 행
            dump_spike_row_to_csv();

            // 다음 사이클 입력 적재
            event_vec_reg = event_next;  // 블로킹 '='
        end

        $fclose(ofile);
        $display("[TB] wrote %0d lines to %s", T, outcsv);
        $finish;
    end
endmodule

`default_nettype wire
