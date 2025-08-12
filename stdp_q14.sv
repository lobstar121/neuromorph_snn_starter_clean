// stdp_q14.sv — Q1.14 STDP (sequential: 1 weight per clock)
// Verilator 친화: 루프 내부 선언/지연할당 피하고, 명시적 캐스팅으로 폭 경고 제거

`timescale 1ns/1ps
`default_nettype none

module stdp_q14 #(
  parameter int F = 48,
  parameter int N = 96,
  parameter int Q = 14,
  localparam int AW = $clog2(F*N)
)(
  input  logic                  clk,
  input  logic                  rst_n,
  input  logic                  enable,

  // spikes (per-timestep pre/post bit-vectors)
  input  logic [F-1:0]          pre_bits,
  input  logic [N-1:0]          post_bits,

  // Q14 parameters
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

  // weight memory R/W
  output logic                  w_we,
  output logic [AW-1:0]         w_addr,
  output logic signed [15:0]    w_wdata,
  input  logic signed [15:0]    w_rdata
);

  // ----------------------------
  // Trace memories (Q14 in 32b)
  // ----------------------------
  logic signed [31:0] x_tr [0:F-1];
  logic signed [31:0] y_tr [0:N-1];

  // ----------------------------
  // Pair indices (f,n)
  // ----------------------------
  localparam int FW = (F<=2)?1 : $clog2(F);
  localparam int NW = (N<=2)?1 : $clog2(N);
  logic [FW-1:0] f_idx;
  logic [NW-1:0] n_idx;

  // 폭 일치 상수
  localparam logic [FW-1:0] Fm1 = logic'(FW'(F-1));
  localparam logic [NW-1:0] Nm1 = logic'(NW'(N-1));

  // ----------------------------
  // Combinational helpers/temps
  // ----------------------------
  // Decay products
  logic signed [63:0] prod_x;      // lambda_x * x_tr[f]
  logic signed [63:0] prod_y;      // lambda_y * y_tr[n]
  logic signed [31:0] bias_x;
  logic signed [31:0] bias_y;
  logic signed [31:0] x_decayed, y_decayed;
  logic signed [31:0] x_upd, y_upd;

  // dW path
  logic               pre_f, post_n;
  logic signed [31:0] x_sel, y_sel;
  logic signed [31:0] dW_q14;
  logic signed [63:0] eta_mul;
  logic signed [31:0] dW_scaled;

  // weight clamp path
  logic signed [31:0] w_old32, w_new32;
  logic signed [15:0] w_clamped;

  // address
  logic [AW-1:0]      addr_comb;

  // ----------------------------
  // Address combine: f*N + n  (명시적 캐스팅으로 폭 경고 제거)
  // ----------------------------
  function automatic [AW-1:0] fn_addr(input int f, input int n);
    logic [31:0] tmp;
    begin
      tmp     = (32'(f) * 32'(N)) + 32'(n);
      fn_addr = AW'(tmp);
    end
  endfunction

  // ----------------------------
  // Combinational math per (f_idx,n_idx)
  // ----------------------------
  always_comb begin
    // 기본
    pre_f   = pre_bits[f_idx];
    post_n  = post_bits[n_idx];

    // --- decay with signed round ---
    prod_x  = $signed(lambda_x) * $signed(x_tr[f_idx]); // 16x32 → 48..64
    prod_y  = $signed(lambda_y) * $signed(y_tr[n_idx]);

    bias_x  = (prod_x >= 0) ? (32'sd1 <<< (Q-1)) : -(32'sd1 <<< (Q-1));
    bias_y  = (prod_y >= 0) ? (32'sd1 <<< (Q-1)) : -(32'sd1 <<< (Q-1));

    // 하위 32비트 사용 + 라운드 후 쉬프트
    x_decayed = ( (prod_x[31:0] + bias_x) >>> Q );
    y_decayed = ( (prod_y[31:0] + bias_y) >>> Q );

    // spike add (Q14)
    x_upd = x_decayed + (pre_f  ? {{16{b_pre[15]}},  b_pre}  : 32'sd0);
    y_upd = y_decayed + (post_n ? {{16{b_post[15]}}, b_post} : 32'sd0);

    // select traces for this pair
    x_sel = x_upd; // (f_idx)의 최신 x
    y_sel = y_upd; // (n_idx)의 최신 y

    // ΔW_q14 = pre?y - post?x
    dW_q14 = (pre_f ? y_sel : 32'sd0) - (post_n ? x_sel : 32'sd0);

    // scale: (eta * dW_q14) >> eta_shift
    eta_mul   = $signed(eta) * $signed(dW_q14);       // 16x32 -> 48..64
    dW_scaled = $signed( (eta_mul >>> eta_shift)[31:0] ); // 64→32 명시 슬라이스

    // weight old & new
    w_old32  = {{16{w_rdata[15]}}, w_rdata};         // sign-extend 16→32
    w_new32  = w_old32 + dW_scaled;

    // clamp to [wmin, wmax] (both Q14 16-bit)
    if (w_new32 > {{16{wmax[15]}}, wmax})
      w_clamped = wmax;
    else if (w_new32 < {{16{wmin[15]}}, wmin})
      w_clamped = wmin;
    else if (w_new32 > 32'sd32767)
      w_clamped = 16'sd32767;
    else if (w_new32 < -32'sd32768)
      w_clamped = -16'sd32768;
    else
      w_clamped = w_new32[15:0];

    // address (인자 캐스팅으로 폭 경고 제거)
    addr_comb = fn_addr(int'(f_idx), int'(n_idx));
  end

  // ----------------------------
  // Sequential: update traces & emit one write per clk
  // ----------------------------
  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      // traces clear
      for (int f = 0; f < F; f++) x_tr[f] = '0;
      for (int n = 0; n < N; n++) y_tr[n] = '0;

      f_idx <= '0;
      n_idx <= '0;

      w_we   <= 1'b0;
      w_addr <= '0;
      w_wdata<= '0;
    end else begin
      // 기본 write 비활성
      w_we   <= 1'b0;
      w_addr <= addr_comb;
      w_wdata<= w_clamped;

      if (enable) begin
        // traces 업데이트 (블로킹 '=')
        x_tr[f_idx] = x_upd;
        y_tr[n_idx] = y_upd;

        // weight write-back (enable_pre/post 조건 적용)
        if ((enable_pre  ? pre_f  : 1'b1) &&
            (enable_post ? post_n : 1'b1)) begin
          w_we <= 1'b1;
        end

        // 인덱스 증가: n → f (폭 맞춘 상수 사용)
        if (n_idx == Nm1) begin
          n_idx <= '0;
          f_idx <= (f_idx == Fm1) ? '0 : (f_idx + 1'b1);
        end else begin
          n_idx <= (n_idx + 1'b1);
        end
      end
    end
  end

endmodule

`default_nettype wire
