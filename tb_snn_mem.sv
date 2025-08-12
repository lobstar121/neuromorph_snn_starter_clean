`timescale 1ns/1ps
`default_nettype none

module tb_snn_mem;
    localparam int F = 48;
    localparam int N = 96;
    localparam int Q = 14;
    localparam int AW = $clog2(F*N);
    parameter  int ALPHA_Q14 = 15474;

    logic clk = 0;  always #1 clk = ~clk;
    logic rstn = 0;

    // I/O
    logic [F-1:0] event_vec_reg, event_next;
    logic [N-1:0] spikes_vec;

    // memories read from files
    logic [F-1:0]        events_mem  [0:65535];
    logic signed [15:0]  weights_mem [0:(F*N)-1];
    logic signed [15:0]  vth_mem     [0:N-1];

    // stdp params / control
    int    T;
    bit    DO_LEARN;

    // STDP params (defaults = SW smoke)
    shortint signed P_eta        = 8;
    byte            P_eta_shift  = 12;
    shortint signed P_lambda_x   = 15565;
    shortint signed P_lambda_y   = 15565;
    shortint signed P_b_pre      = 1024;
    shortint signed P_b_post     = 1024;
    shortint signed P_wmin       = -16384;
    shortint signed P_wmax       =  16384;
    bit            P_en_pre      = 1'b1;
    bit            P_en_post     = 1'b1;

    // files
    string whex, vthx, evhex, outcsv, wout;

    // DUT
    logic                 stdp_enable;
    logic [F-1:0]         stdp_pre_bits;
    logic [N-1:0]         stdp_post_bits;

    logic [AW-1:0]        rb_addr;
    logic signed [15:0]   rb_data;

    snn_core #(.F(F), .N(N), .Q(Q), .ALPHA_Q14(ALPHA_Q14)) dut (
        .clk        (clk),
        .rstn       (rstn),
        .event_vec  (event_vec_reg),
        .spikes_vec (spikes_vec),

        .stdp_enable      (stdp_enable),
        .stdp_pre_bits    (stdp_pre_bits),
        .stdp_post_bits   (stdp_post_bits),
        .stdp_eta         (P_eta),
        .stdp_eta_shift   (P_eta_shift),
        .stdp_lambda_x    (P_lambda_x),
        .stdp_lambda_y    (P_lambda_y),
        .stdp_b_pre       (P_b_pre),
        .stdp_b_post      (P_b_post),
        .stdp_wmin        (P_wmin),
        .stdp_wmax        (P_wmax),
        .stdp_enable_pre  (P_en_pre),
        .stdp_enable_post (P_en_post),

        .rb_addr   (rb_addr),
        .rb_data   (rb_data)
    );

    // utils
    task load_plusargs();
        if (!$value$plusargs("WHEX=%s", whex))   whex   = "artifacts/weights.hex";
        if (!$value$plusargs("VTH=%s",  vthx))   vthx   = "artifacts/vth.hex";
        if (!$value$plusargs("EVHEX=%s", evhex)) evhex  = "artifacts/events_ref.mem";
        if (!$value$plusargs("OUT=%s",  outcsv)) outcsv = "artifacts/spikes_hw.csv";
        if (!$value$plusargs("WOUT=%s", wout))   wout   = "artifacts/weights_learned_rtl.hex";
        if (!$value$plusargs("T=%d",    T))      T      = 76;
        int l; if ($value$plusargs("LEARN=%d", l)) DO_LEARN = (l!=0);
        // optional param overrides
        void'($value$plusargs("ETA=%d",       P_eta));
        int tmp;
        if ($value$plusargs("ETA_SHIFT=%d",   tmp)) P_eta_shift = tmp[7:0];
        void'($value$plusargs("LAMBDA_X=%d",  P_lambda_x));
        void'($value$plusargs("LAMBDA_Y=%d",  P_lambda_y));
        void'($value$plusargs("B_PRE=%d",     P_b_pre));
        void'($value$plusargs("B_POST=%d",    P_b_post));
        void'($value$plusargs("WMIN=%d",      P_wmin));
        void'($value$plusargs("WMAX=%d",      P_wmax));
        int ep, ept;
        if ($value$plusargs("EN_PRE=%d",  ep))  P_en_pre  = (ep!=0);
        if ($value$plusargs("EN_POST=%d", ept)) P_en_post = (ept!=0);

        $display("[TB] WHEX=%s  VTH=%s  EVHEX=%s", whex, vthx, evhex);
        $display("[TB] OUT=%s  WOUT=%s  T=%0d  LEARN=%0d", outcsv, wout, T, DO_LEARN);
        $display("[TB] ETA=%0d SHIFT=%0d Lx=%0d Ly=%0d bpre=%0d bpost=%0d wmin=%0d wmax=%0d enP=%0d enQ=%0d",
                  P_eta, P_eta_shift, P_lambda_x, P_lambda_y, P_b_pre, P_b_post, P_wmin, P_wmax, P_en_pre, P_en_post);
    endtask

    task load_mems();
        $display("[TB] loading %s", whex);  $readmemh(whex, weights_mem);
        $display("[TB] loading %s", vthx);  $readmemh(vthx,  vth_mem);
        $display("[TB] loading %s", evhex); $readmemh(evhex, events_mem);
        // poke into DUT ROMs
        for (int i = 0; i < F*N; i++) dut.weights_rom[i] = weights_mem[i];
        for (int i = 0; i < N;   i++) dut.vth_rom[i]     = vth_mem[i];
    endtask

    integer ofile;

    task dump_spike_row_to_csv();
        for (int n = 0; n < N; n++) begin
            $fwrite(ofile, "%0d", spikes_vec[n] ? 1 : 0);
            if (n != N-1) $fwrite(ofile, ",");
        end
        $fwrite(ofile, "\n");
    endtask

    // readback dump of weights_ram via rb_* ports
    task dump_weights_hex(string path);
        integer wf = $fopen(path, "w");
        if (wf == 0) begin
            $display("[TB][ERR] cannot open %s", path);
            return;
        end
        for (int i = 0; i < F*N; i++) begin
            rb_addr = i[AW-1:0];
            @(posedge clk);
            int signed w = rb_data;
            int unsigned u = (w < 0) ? (w + (1<<16)) : w;
            $fwrite(wf, "%04x\n", u[15:0]);
        end
        $fclose(wf);
        $display("[TB] dumped learned weights to %s", path);
    endtask

    // main
    initial begin
        stdp_enable   = 1'b0;
        stdp_pre_bits = '0;
        stdp_post_bits= '0;
        rb_addr       = '0;

        load_plusargs();
        load_mems();

        ofile = $fopen(outcsv, "w");
        if (ofile == 0) begin
            $display("[TB][ERROR] cannot open %s", outcsv);
            $finish;
        end

        event_vec_reg = '0; event_next = '0;
        repeat (4) @(posedge clk);
        rstn = 1; @(posedge clk);

        // warm-up: preload t=0
        event_next = events_mem[0];
        @(posedge clk);
        event_vec_reg = event_next;

        // main loop
        for (int t = 0; t < T; t++) begin
            // prepare next
            if (t+1 < T) event_next = events_mem[t+1];
            else         event_next = '0;

            @(posedge clk);
            dump_spike_row_to_csv();

            // STDP pass
            if (DO_LEARN) begin
                stdp_pre_bits  = event_vec_reg;
                stdp_post_bits = spikes_vec;
                stdp_enable    = 1'b1;
                // 한 스텝당 전체 가중치 1회 스캔
                for (int k = 0; k < F*N; k++) @(posedge clk);
                stdp_enable    = 1'b0;
                @(posedge clk);
            end

            // next input
            event_vec_reg = event_next;
        end

        $fclose(ofile);
        $display("[TB] wrote %0d lines to %s", T, outcsv);

        if (DO_LEARN) begin
            dump_weights_hex(wout);
        end
        $finish;
    end
endmodule

`default_nettype wire
