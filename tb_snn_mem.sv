// tb_snn_mem.sv — TB with STDP on/off + weight RAM dump

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
    always #1 clk = ~clk;  // 1ns 토글

    // ---- 입출력 레지스터/버퍼 ----
    logic [F-1:0] event_vec_reg;
    logic [F-1:0] event_next;
    logic [N-1:0] spikes_vec;

    // ---- plusargs & 파일 핸들 ----
    string whex, vthx, evhex, outcsv, wouthex;
    integer ofile, wfile;
    int T, STDP_EN;

    // ---- TB 로드 메모리 ----
    logic [F-1:0]        events_mem  [0:65535];      // stimulus bits
    logic signed [15:0]  weights_mem [0:(F*N)-1];    // Q1.14
    logic signed [15:0]  vth_mem     [0:N-1];        // Q1.14

    // ---- DUT ----
    snn_core #(
        .F(F), .N(N), .Q(Q), .ALPHA_Q14(ALPHA_Q14)
    ) dut (
        .clk         (clk),
        .rstn        (rstn),
        .event_vec   (event_vec_reg),
        .spikes_vec  (spikes_vec),
        .stdp_enable (STDP_EN != 0)  // STDP on/off
    );

    // ------------------------
    // 유틸 태스크
    // ------------------------
    task load_plusargs();
        if (!$value$plusargs("WHEX=%s", whex))     whex     = "artifacts/weights.hex";
        if (!$value$plusargs("VTH=%s",  vthx))     vthx     = "artifacts/vth.hex";
        if (!$value$plusargs("EVHEX=%s", evhex))   evhex    = "artifacts/events_ref.mem";
        if (!$value$plusargs("OUT=%s",  outcsv))   outcsv   = "artifacts/spikes_hw.csv";
        if (!$value$plusargs("WOUT=%s", wouthex))  wouthex  = "artifacts/weights_learned_rtl.hex";
        if (!$value$plusargs("T=%d",    T))        T        = 76;
        if (!$value$plusargs("STDP=%d", STDP_EN))  STDP_EN  = 0;
        $display("[TB] WHEX=%s  VTH=%s  EVHEX=%s  OUT=%s  WOUT=%s  T=%0d  STDP=%0d",
                 whex, vthx, evhex, outcsv, wouthex, T, STDP_EN);
    endtask

    task load_mems();
        $display("[TB] loading %s", whex);
        $readmemh(whex, weights_mem);
        $display("[TB] loading %s", vthx);
        $readmemh(vthx, vth_mem);
        $display("[TB] loading %s", evhex);
        $readmemh(evhex, events_mem);

        // DUT 내부 공개 ROM으로 복사 (리셋 직전에 준비)
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

    task dump_weights_ram_hex();
        wfile = $fopen(wouthex, "w");
        if (wfile == 0) begin
            $display("[TB][ERROR] cannot open %s for write", wouthex);
            return;
        end
        for (int i = 0; i < F*N; i++) begin
            int signed x = dut.weights_ram[i];
            if (x < 0) x = (1<<16) + x;
            $fdisplay(wfile, "%04x", x[15:0]);
        end
        $fclose(wfile);
        $display("[TB] dumped weights RAM -> %s", wouthex);
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

        // Warm-up 1 tick: t=0 준비 → 다음 엣지에 적재
        event_next = events_mem[0];
        @(posedge clk);
        event_vec_reg = event_next;   // blocking

        // 본 루프
        for (int t = 0; t < T; t++) begin
            if (t+1 < T) event_next = events_mem[t+1];
            else         event_next = '0;

            @(posedge clk);
            dump_spike_row_to_csv();          // t번째 결과
            event_vec_reg = event_next;       // 다음 입력 적재
        end

        $fclose(ofile);
        if (STDP_EN != 0) dump_weights_ram_hex();
        $display("[TB] wrote %0d lines to %s", T, outcsv);
        $finish;
    end
endmodule

`default_nettype wire
