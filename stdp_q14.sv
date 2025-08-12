// stdp_q14.sv — Q14 STDP skeleton (no-op)
// 아직 인스턴스하지 않더라도 컴파일만 통과하도록 최소화

`timescale 1ns/1ps
`default_nettype none

module stdp_q14 #(
  parameter int F = 48,
  parameter int N = 96,
  parameter int Q = 14
)(
  input  logic                  clk,
  input  logic                  rst_n,
  input  logic                  enable,

  input  logic [F-1:0]          pre_bits,
  input  logic [N-1:0]          post_bits,

  input  logic signed [15:0]    eta,
  input  logic        [7:0]     eta_shift,
  input  logic signed [15:0]    lambda_x,
  input  logic signed [15:0]    lambda_y,
  input  logic signed [15:0]    b_pre,
  input  logic signed [15:0]    b_post,
  input  logic signed [15:0]    wmin,
  input  logic signed [15:0]    wmax,
  input  logic                  enable_pre,
  input  logic                  enable_post,

  output logic                  w_we,
  output logic [$clog2((F*N)<=1?2:(F*N))-1:0] w_addr,
  output logic signed [15:0]    w_wdata
);
  // no-op tie-offs
  always_comb begin
    w_we   = 1'b0;
    w_addr = '0;
    w_wdata = '0;
  end
endmodule

`default_nettype wire
