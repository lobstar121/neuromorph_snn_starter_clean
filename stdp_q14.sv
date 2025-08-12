// stdp_q14.sv — Q14 STDP skeleton (no-op)
// 앞으로 실제 로직을 채울 자리. 현재는 write-back 모두 비활성.

`timescale 1ns/1ps
`default_nettype none

module stdp_q14 #(
  parameter int F = 48,
  parameter int N = 96,
  parameter int Q = 14
)(
  input  logic                  clk,
  input  logic                  rstn,        // 일관된 이름 사용 (rstn)

  input  logic                  enable,

  // 한 사이클 pre/post 스파이크 비트
  input  logic [F-1:0]          pre_bits,
  input  logic [N-1:0]          post_bits,

  // Q14 파라미터
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

  // 가중치 메모리 write-back 인터페이스
  output logic                  w_we,
  output logic [$clog2(F*N)-1:0] w_addr,     // AW를 직접 사용해도 됨
  output logic signed [15:0]    w_wdata
);

  // (선택) 본문 내에서의 명시적 로컬파라미터
  localparam int AW = $clog2(F*N);

  // --- skeleton: 모든 출력은 0, 내부 상태 없음 ---
  assign w_we    = 1'b0;
  assign w_addr  = '0;
  assign w_wdata = '0;

endmodule

`default_nettype wire
