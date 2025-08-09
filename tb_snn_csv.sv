// tb_snn_csv.sv -- Verilator-friendly testbench
// Usage (example):
//   verilator -sv --binary tb_snn_csv.sv snn_core.sv
//   ./tb_snn_csv +XCSV=artifacts/X_events_ref.csv +WHEX=artifacts/weights.hex +VTH=artifacts/vth.hex +OUT=artifacts/spikes_hw.csv

module tb_snn_csv;
    localparam int F = 48;
    localparam int N = 96;
    localparam int Q = 14;

    // DUT
    logic clk=0, rstn=0;
    logic [F-1:0] event_vec;
    logic [N-1:0] spikes_vec;

    snn_core #(.F(F), .N(N), .Q(Q)) dut (
        .clk(clk), .rstn(rstn),
        .event_vec(event_vec),
        .spikes_vec(spikes_vec)
    );

    // clock
    always #1 clk = ~clk; // 500kHz in sim units

    // memories
    // readmemh for weights/vth
    string whex, vthx, xcsv, outcsv;
    integer wf, vf, xf, of;
    logic signed [15:0] weights_mem [0:(F*N)-1];
    logic signed [15:0] vth_mem     [0:N-1];

    // Give DUT roms the same data (using hierarchical access)
    // Verilator allows hierarchical reference in testbench
    task load_hexes();
        if (!$value$plusargs("WHEX=%s", whex)) whex = "artifacts/weights.hex";
        if (!$value$plusargs("VTH=%s", vthx))  vthx = "artifacts/vth.hex";
        $display("[TB] loading %s", whex);
        $readmemh(whex, weights_mem);
        $display("[TB] loading %s", vthx);
        $readmemh(vthx, vth_mem);
        // copy into DUT's roms
        for (int i=0;i<F*N;i++) dut.weights_rom[i] = weights_mem[i];
        for (int i=0;i<N;i++)   dut.vth_rom[i]     = vth_mem[i];
    endtask

    // CSV reader: each line has F integers (0/1) separated by commas
    function int read_csv_line(input integer fh, output logic [F-1:0] bits);
        string line;
        if ($feof(fh)) return 0;
        void'($fgets(line, fh));
        // parse
        int value;
        int idx = 0;
        bits = '0;
        for (int f=0; f<F; f++) begin
            if ($sscanf(line, "%d%[, ]%s", value, line, line) >= 1) begin
                if (value != 0) bits[f] = 1'b1;
            end else begin
                // Fallback simple parse: try comma split
                // (Verilator's $sscanf can be finicky; leaving robust path above)
            end
        end
        return 1;
    endfunction

    initial begin
        if (!$value$plusargs("XCSV=%s", xcsv)) xcsv = "artifacts/X_events_ref.csv";
        if (!$value$plusargs("OUT=%s",  outcsv)) outcsv = "artifacts/spikes_hw.csv";

        load_hexes();

        xf = $fopen(xcsv, "r");
        if (xf == 0) begin
            $display("[TB][ERROR] cannot open %s", xcsv);
            $finish;
        end
        of = $fopen(outcsv, "w");
        if (of == 0) begin
            $display("[TB][ERROR] cannot open %s for write", outcsv);
            $finish;
        end

        // reset
        repeat (4) @(posedge clk);
        rstn = 1;
        @(posedge clk);

        int t = 0;
        // For each line in CSV, drive event_vec, wait one clk, dump spikes
        while (! $feof(xf)) begin
            logic [F-1:0] evb;
            // simplistic CSV read: read a whole line and tokenize manually
            string line;
            void'($fgets(line, xf));
            // tokenize by commas
            evb = '0;
            int val, pos;
            pos = 0;
            for (int f=0; f<F; f++) begin
                int code, tmp;
                code = $sscanf(line, "%d,%s", val, line);
                if (code == 0) begin
                    // last field without comma
                    void'($sscanf(line, "%d", val));
                end
                if (val != 0) evb[f] = 1'b1;
            end

            event_vec = evb;
            @(posedge clk); // process this step

            // dump spikes line as CSV
            for (int n=0; n<N; n++) begin
                $fwrite(of, "%0d", spikes_vec[n] ? 1 : 0);
                if (n != N-1) $fwrite(of, ",");
            end
            $fwrite(of, "\n");
            t++;
        end

        $fclose(xf);
        $fclose(of);
        $display("[TB] Wrote %s lines to %s", t, outcsv);
        $finish;
    end
endmodule
