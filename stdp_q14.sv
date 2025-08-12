// stdp_q14.sv — Q14 STDP skeleton (no-op)
// 기능은 0으로 tie-off. 후속 단계에서 실제 로직 채움.
module stdp_q14 #(
  parameter int F = 48,
  parameter int N = 96,
  parameter int Q = 14,
  localparam int AW = $clog2(F*N)
)(
  input  logic                  clk,
  input  logic                  rst_n,
  input  logic                  enable,

  // spikes (한 사이클의 pre/post 스파이크 비트)
  input  logic [F-1:0]          pre_bits,
  input  logic [N-1:0]          post_bits,

  // Q14 파라미터들 (부호 16비트 해석)
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
  output logic [AW-1:0]         w_addr,
  output logic signed [15:0]    w_wdata
);
  // --- skeleton: 모든 출력은 0, 내부 상태 없음 ---
  assign w_we    = 1'b0;
  assign w_addr  = '0;
  assign w_wdata = '0;

endmodule
