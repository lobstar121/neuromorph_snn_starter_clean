// lif_neuron.sv -- Q1.14 LIF neuron (combinational update helper)
// Note: For simplicity we don't instantiate this per-neuron; snn_core does vector updates.

module sat16 (
    input  logic signed [31:0] in32,
    output logic signed [15:0] out16
);
    logic signed [31:0] minv = -32'sd32768;
    logic signed [31:0] maxv =  32'sd32767;
    always @* begin
        if (in32 > maxv) out16 = 16'sd32767;
        else if (in32 < minv) out16 = -16'sd32768;
        else out16 = in32[15:0];
    end
endmodule
