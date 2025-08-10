`timescale 1ns/1ps
`default_nettype none

module tb_snn_mem;
    localparam int F = 48;
    localparam int N = 96;
    localparam int Q = 14;
    parameter  int ALPHA_Q14 = 15474;

    // DUT I/O
    logic clk = 0;
    logic rstn = 0;

    // 파이프라인 입력(레지스터) & 다음 자극 버퍼
    logic [F-1:0] event_vec_reg;
    logic [F-1:0] event_next;
    logic [N-1:0] spikes_vec;

    // DUT
    snn_core #(.F(F), .N(N), .Q(Q), .ALPHA_Q14(ALPHA_Q14)) dut (
        .clk       (clk),
        .rstn      (rstn),
        .event_vec (event_vec_reg),
        .spikes_vec(spikes_vec)
    );

    // 1ns 토글 클럭 (Verilator 5.x는 --timing 필요)
    always #1 clk = ~clk;

    // plusargs & memories
    string whex, vthx, evhex, outcsv;
    integer ofile;
    int T;

    logic [F-1:0]        events_mem [0:65535];
    logic signed [15:0]  weights_mem [0:(F*N)-1];
    logic signed [15:0]  vth_mem     [0:N-1];

    // ----- 유틸 태스크 -----
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

        // dut 내부 ROM에 복사
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

    // ----- 메인 시퀀스 -----
    initial begin
        load_plusargs();
        load_mems();

        ofile = $fopen(outcsv, "w");
        if (ofile == 0) begin
            $display("[TB][ERROR] cannot open %s for write", outcsv);
            $finish;
        end

        // 초기값
        event_vec_reg = '0;
        event_next    = '0;

        // 리셋
        repeat (4) @(posedge clk);
        rstn = 1;
        @(posedge clk);

        // ===== Warm-up 1 tick =====
        // t=0 자극을 먼저 준비하고, 다음 엣지에서 레지스터에 싣기
        event_next = events_mem[0];
        @(posedge clk);
        event_vec_reg = event_next;   // <<<< initial 블록에서는 블로킹 '=' 사용

        // ===== 본 루프 =====
        for (int t = 0; t < T; t++) begin
            // 다음 자극 준비
            if (t+1 < T) event_next = events_mem[t+1];
            else         event_next = '0;

            @(posedge clk);
            // 이 시점에서 spikes_vec = 직전 사이클 event_vec_reg의 결과 → t번째 결과
            dump_spike_row_to_csv();

            // 다음 사이클 입력을 레지스터에 적재
            event_vec_reg = event_next;  // <<<< 역시 '='
        end

        $fclose(ofile);
        $display("[TB] wrote %0d lines to %s", T, outcsv);
        $finish;
    end
endmodule

`default_nettype wire
