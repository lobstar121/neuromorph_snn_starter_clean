`timescale 1ns/1ps
`default_nettype none

module tb_snn_mem;
    localparam int F = 48;
    localparam int N = 96;
    localparam int Q = 14;

    // DUT
    logic clk = 0, rstn = 0;
    logic [F-1:0] event_vec;
    logic [N-1:0] spikes_vec;

    snn_core #(.F(F), .N(N), .Q(Q)) dut (
        .clk(clk), .rstn(rstn),
        .event_vec(event_vec),
        .spikes_vec(spikes_vec)
    );

    // clock
    always #1 clk = ~clk;

    // plusargs & memories
    string whex, vthx, evhex, outcsv;
    integer ofile;
    int T;

    logic [F-1:0]        events_mem [0:65535];
    logic signed [15:0]  weights_mem [0:(F*N)-1];
    logic signed [15:0]  vth_mem     [0:N-1];

    task load_plusargs();
        if (!$value$plusargs("WHEX=%s", whex))  whex  = "artifacts/weights.hex";
        if (!$value$plusargs("VTH=%s",  vthx))  vthx  = "artifacts/vth.hex";
        if (!$value$plusargs("EVHEX=%s", evhex)) evhex = "artifacts/events_ref.mem";
        if (!$value$plusargs("OUT=%s",  outcsv)) outcsv = "artifacts/spikes_hw.csv";
        if (!$value$plusargs("T=%d", T)) T = 76;
        $display("[TB] WHEX=%s  VTH=%s  EVHEX=%s  OUT=%s  T=%0d", whex, vthx, evhex, outcsv, T);
    endtask

    task load_mems();
        $display("[TB] loading %s", whex);
        $readmemh(whex, weights_mem);
        $display("[TB] loading %s", vthx);
        $readmemh(vthx, vth_mem);
        $display("[TB] loading %s", evhex);
        $readmemh(evhex, events_mem);

        // copy into DUT internal ROMs
        for (int i = 0; i < F*N; i++) dut.weights_rom[i] = weights_mem[i];
        for (int i = 0; i < N;   i++) dut.vth_rom[i]     = vth_mem[i];
    endtask

    initial begin
        load_plusargs();
        load_mems();

        ofile = $fopen(outcsv, "w");
        if (ofile == 0) begin
            $display("[TB][ERROR] cannot open %s for write", outcsv);
            $finish;
        end

        // reset
        repeat (4) @(posedge clk);
        rstn = 1;
        @(posedge clk);

        // run T steps
        for (int t = 0; t < T; t++) begin
            event_vec = events_mem[t];
            @(posedge clk);
            // dump spikes row
            for (int n = 0; n < N; n++) begin
                $fwrite(ofile, "%0d", spikes_vec[n] ? 1 : 0);
                if (n != N-1) $fwrite(ofile, ",");
            end
            $fwrite(ofile, "\n");
        end

        $fclose(ofile);
        $display("[TB] wrote %0d lines to %s", T, outcsv);
        $finish;
    end
endmodule

`default_nettype wire
