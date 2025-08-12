// stdp_q14.sv — Q14 STDP (sequential, one write per cycle)
// - 한 타임스텝(enable=1) 입력 시: x[F] → y[N] → W[F×N] 순으로 업데이트
// - Q14 부호 반올림 쉬프트 사용(파이썬과 동일 동작)
// - 가중치는 외부에서 읽어(w_rdata) 들어오고, 본 모듈이 클램프 후 write-back

`timescale 1ns/1ps
`default_nettype none

module stdp_q14 #(
  parameter int F = 48,
  parameter int N = 96,
  parameter int Q = 14,
  localparam int AW = (F*N <= 1) ? 1 : $clog2(F*N)
)(
  input  logic                  clk,
  input  logic                  rst_n,

  // 한 타임스텝 펄스 (pre/post가 유효한 사이클에 1)
  input  logic                  enable,

  // spikes (이 타임스텝의 pre/post 스파이크 비트)
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

  // 가중치 메모리 read/write 인터페이스
  input  logic signed [15:0]    w_rdata,       // 현재 weight 값
  output logic                  w_we,
  output logic [AW-1:0]         w_addr,
  output logic signed [15:0]    w_wdata
);

  // ========= 내부 상태 =========
  // 추적 변수는 약간 여유 있는 폭으로 유지(Q14 계산 중 임시 오버플로 방지)
  logic signed [31:0] x_trace [0:F-1];  // presyn traces (Q14 해석)
  logic signed [31:0] y_trace [0:N-1];  // postsyn traces (Q14 해석)

  // 현재 타임스텝의 pre/post를 래치(여러 사이클에 걸친 순차 처리용)
  logic [F-1:0] pre_latched;
  logic [N-1:0] post_latched;

  // 인덱스 & FSM
  typedef enum logic [2:0] {S_IDLE, S_UPD_X, S_UPD_Y, S_W_F, S_W_N} state_t;
  state_t st;

  integer i_f;
  integer i_n;

  // ========= 보조 함수: 부호 반올림 쉬프트(Q14) =========
  function automatic logic signed [31:0] sround_rshift_q(
      input logic signed [31:0] v,
      input int unsigned        q
  );
    logic signed [31:0] bias;
    begin
      bias = (v >= 0) ? (32'sd1 <<< (q-1)) : -(32'sd1 <<< (q-1));
      sround_rshift_q = (v + bias) >>> q;
    end
  endfunction

  // ========= 기본 출력 =========
  always_comb begin
    w_we    = 1'b0;
    w_addr  = '0;
    w_wdata = '0;
  end

  // ========= 메인 FSM =========
  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      // 초기화
      for (int f = 0; f < F; f++) x_trace[f] = '0;       // 블로킹 '=' 사용 (Verilator 권고)
      for (int n = 0; n < N; n++) y_trace[n] = '0;
      pre_latched <= '0;
      post_latched <= '0;
      st <= S_IDLE;
      i_f = 0;
      i_n = 0;
    end else begin
      unique case (st)
        // ----------------- 대기: enable 펄스 오면 pre/post 래치 -----------------
        S_IDLE: begin
          if (enable) begin
            pre_latched  <= pre_bits;
            post_latched <= post_bits;
            i_f = 0;
            st  <= S_UPD_X;
          end
        end

        // ----------------- x[F] 추적 업데이트 -----------------
        S_UPD_X: begin
          // x = round((lambda_x*x)>>Q) + b_pre*pre_bit
          // enable_pre가 꺼져 있으면 pre 기여 0
          logic signed [31:0] x_old, mul32, decayed, add_pre;
          x_old   = x_trace[i_f];
          mul32   = $signed(lambda_x) * x_old;            // Q14 * Q14(해석) → Q28
          decayed = sround_rshift_q(mul32, Q);            // 다시 Q14 범위
          add_pre = (enable_pre && pre_latched[i_f]) ? $signed(b_pre) : 32'sd0;
          x_trace[i_f] = decayed + add_pre;               // '=' (블로킹)

          if (i_f == F-1) begin
            i_n = 0;
            st  <= S_UPD_Y;
          end else begin
            i_f = i_f + 1;
          end
        end

        // ----------------- y[N] 추적 업데이트 -----------------
        S_UPD_Y: begin
          // y = round((lambda_y*y)>>Q) + b_post*post_bit
          logic signed [31:0] y_old, mul32, decayed, add_post;
          y_old   = y_trace[i_n];
          mul32   = $signed(lambda_y) * y_old;            // Q28
          decayed = sround_rshift_q(mul32, Q);            // Q14
          add_post= (enable_post && post_latched[i_n]) ? $signed(b_post) : 32'sd0;
          y_trace[i_n] = decayed + add_post;

          if (i_n == N-1) begin
            // 가중치 업데이트 시작
            i_f = 0;
            st  <= S_W_F;
          end else begin
            i_n = i_n + 1;
          end
        end

        // ----------------- 가중치 업데이트: 바깥 루프 f -----------------
        S_W_F: begin
          i_n = 0;
          st  <= S_W_N;
        end

        // ----------------- 가중치 업데이트: 안쪽 루프 n (한 사이클에 한 주소 write) -----------------
        S_W_N: begin
          // ΔW = (η * (pre? y[n] : 0  −  post? x[f] : 0)) >> eta_shift
          logic signed [31:0] term_pre, term_post, dW32, dW_scaled;
          term_pre  = (pre_latched[i_f])  ? y_trace[i_n] : 32'sd0;
          term_post = (post_latched[i_n]) ? x_trace[i_f] : 32'sd0;
          dW32      = $signed(eta) * (term_pre - term_post);
          // eta_shift는 0~255, 범위를 보호
          dW_scaled = (eta_shift == 0) ? dW32 : (dW32 >>> eta_shift);

          // 현재 weight 읽고, 더하고, 클램프
          logic signed [31:0] w_old, w_new32;
          logic signed [15:0] w_clamped;
          w_old   = {{16{w_rdata[15]}}, w_rdata};     // sign-extend 16->32
          w_new32 = w_old + dW_scaled;

          // 클램프 범위는 16비트로 들어오므로, 32->16 변환 전에 비교
          if (w_new32 > {{16{wmax[15]}}, wmax})      w_clamped = wmax;
          else if (w_new32 < {{16{wmin[15]}}, wmin}) w_clamped = wmin;
          else                                       w_clamped = w_new32[15:0];

          // write-back
          w_addr <= AW'(i_f*N + i_n);
          w_wdata <= w_clamped;
          w_we   <= 1'b1;

          // 다음 인덱스
          if (i_n == N-1) begin
            if (i_f == F-1) begin
              st <= S_IDLE;          // 한 타임스텝 완료
            end else begin
              i_f <= i_f + 1;
              st  <= S_W_F;
            end
          end else begin
            i_n <= i_n + 1;
          end
        end

        default: st <= S_IDLE;
      endcase
    end
  end

endmodule

`default_nettype wire
