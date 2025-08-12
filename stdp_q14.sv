// stdp_q14.sv — Q14 STDP, sequential update (1 weight per clock)
// Processes one postsyn neuron n, iterates presyn f=0..F-1
`timescale 1ns/1ps
`default_nettype none

module stdp_q14 #(
  parameter int F = 48,
  parameter int N = 96,
  parameter int Q = 14
)(
  input  logic                  clk,
  input  logic                  rst_n,
  input  logic                  enable,         // pulse: start one learning step

  // spikes for this timestep
  input  logic [F-1:0]          pre_bits,
  input  logic [N-1:0]          post_bits,

  // Q14 parameters
  input  logic signed [15:0]    eta,            // learning rate (int)
  input  logic        [7:0]     eta_shift,      // right shift after eta multiply
  input  logic signed [15:0]    lambda_x,       // decay Q14
  input  logic signed [15:0]    lambda_y,       // decay Q14
  input  logic signed [15:0]    b_pre,          // bump Q14 when pre spikes
  input  logic signed [15:0]    b_post,         // bump Q14 when post spikes
  input  logic signed [15:0]    wmin,           // clamp
  input  logic signed [15:0]    wmax,           // clamp
  input  logic                  enable_pre,     // apply +pre⊗y term
  input  logic                  enable_post,    // apply -x⊗post term

  // weight memory read/modify/write interface
  output logic                  w_we,
  output logic [$clog2(F*N)-1:0] w_addr,
  output logic signed [15:0]    w_wdata,
  input  logic signed [15:0]    w_rdata        // current weight at w_addr
);

  // ---------------------------
  // Internal state
  // ---------------------------
  typedef enum logic [1:0] {S_IDLE, S_YUPD, S_FLOOP} state_t;
  state_t st;

  // latched spikes for this step
  logic [F-1:0] pre_latched;
  logic [N-1:0] post_latched;

  // traces (Q14, keep as 32b for headroom)
  logic signed [31:0] x_trace [0:F-1]; // presyn traces
  logic signed [31:0] y_trace [0:N-1]; // postsyn traces

  // visited flags per step so we update each trace once
  logic visited_f [0:F-1];
  logic visited_y [0:N-1];

  // indices
  integer n_idx;
  integer f_idx;

  // helpers
  localparam int QP = Q;

  function automatic logic signed [31:0] q14_round_shift(input logic signed [63:0] v);
    logic signed [63:0] bias;
    begin
      bias = (v >= 0) ? (64'sd1 <<< (QP-1)) : -(64'sd1 <<< (QP-1));
      q14_round_shift = (v + bias) >>> QP;
    end
  endfunction

  // one-step y[n] update (decay+spike bump) in place
  task automatic upd_y(input integer n);
    logic signed [63:0] prod;
    logic signed [31:0] decayed;
    begin
      prod    = $signed(lambda_y) * $signed(y_trace[n]); // 16x32->48 (keep in 64)
      decayed = q14_round_shift(prod);
      y_trace[n] = decayed + (post_latched[n] ? $signed({{16{b_post[15]}}, b_post}) : 32'sd0);
    end
  endtask

  // one-step x[f] update (decay+spike bump) in place
  task automatic upd_x(input integer f);
    logic signed [63:0] prod;
    logic signed [31:0] decayed;
    begin
      prod    = $signed(lambda_x) * $signed(x_trace[f]);
      decayed = q14_round_shift(prod);
      x_trace[f] = decayed + (pre_latched[f] ? $signed({{16{b_pre[15]}}, b_pre}) : 32'sd0);
    end
  endtask

  // compute dW (Q14) for pair (f_idx, n_idx)
  function automatic logic signed [31:0] dW_q14_fn(
      input logic pre_b, input logic post_b,
      input logic signed [31:0] x_f, input logic signed [31:0] y_n
  );
    logic signed [31:0] pre_term;
    logic signed [31:0] post_term;
    begin
      pre_term  = (enable_pre  && pre_b)  ? y_n : 32'sd0;
      post_term = (enable_post && post_b) ? x_f : 32'sd0;
      dW_q14_fn = pre_term - post_term; // still Q14
    end
  endfunction

  // ---------------------------
  // FSM
  // ---------------------------
  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      st           <= S_IDLE;
      pre_latched  <= '0;
      post_latched <= '0;
      n_idx        <= 0;
      f_idx        <= 0;
      w_we         <= 1'b0;
      w_addr       <= '0;
      w_wdata      <= 16'sd0;
      // clear traces / visited
      for (int f = 0; f < F; f++) begin
        x_trace[f]  <= 32'sd0; // blocking/nb ok at reset; non-array inside loop -> '<=' is fine
        visited_f[f] <= 1'b0;
      end
      for (int n = 0; n < N; n++) begin
        y_trace[n]  <= 32'sd0;
        visited_y[n] <= 1'b0;
      end
    end else begin
      // default
      w_we <= 1'b0;

      unique case (st)
        S_IDLE: begin
          if (enable) begin
            pre_latched  <= pre_bits;
            post_latched <= post_bits;
            // reset step indices and visited flags
            n_idx <= 0;
            f_idx <= 0;
            for (int f = 0; f < F; f++) visited_f[f] <= 1'b0;
            for (int n = 0; n < N; n++) visited_y[n] <= 1'b0;
            st    <= S_YUPD;
          end
        end

        // update y[n_idx] once per step, then start f-loop
        S_YUPD: begin
          if (!visited_y[n_idx]) begin
            upd_y(n_idx);
            visited_y[n_idx] <= 1'b1;
          end
          f_idx <= 0;
          st    <= S_FLOOP;
        end

        // iterate f = 0..F-1; update x[f] on first touch; then RMW weight
        S_FLOOP: begin
          // decay/bump x[f_idx] once per step
          if (!visited_f[f_idx]) begin
            upd_x(f_idx);
            visited_f[f_idx] <= 1'b1;
          end

          // address = f*N + n
          w_addr <= f_idx * N + n_idx;

          // compute dW_scaled and write new weight
          // widen to 64 for eta multiply, then arithmetic shift by eta_shift
          logic signed [31:0] dW_q14;
          logic signed [63:0] eta_mul;
          logic signed [31:0] dW_scaled;
          logic signed [31:0] w_old32, w_new32;
          logic signed [15:0] w_clamped;

          dW_q14  = dW_q14_fn(pre_latched[f_idx], post_latched[n_idx],
                              x_trace[f_idx], y_trace[n_idx]);

          eta_mul  = $signed({{48{eta[15]}}, eta}) * $signed(dW_q14); // 16x32->48 -> put into 64
          // arithmetic right shift with sign
          dW_scaled = eta_mul >>> eta_shift;

          // read-modify-write
          w_old32 = {{16{w_rdata[15]}}, w_rdata};
          w_new32 = w_old32 + dW_scaled;

          // clamp to int16
          if (w_new32 > 32'sd32767)       w_clamped = 16'sd32767;
          else if (w_new32 < -32'sd32768) w_clamped = 16'sd-32768;
          else                             w_clamped = w_new32[15:0];

          // optional extra clamp to wmin/wmax (both int16)
          if (w_clamped > wmax)      w_clamped = wmax;
          else if (w_clamped < wmin) w_clamped = wmin;

          // commit
          w_wdata <= w_clamped;
          w_we    <= 1'b1;

          // advance f / n
          if (f_idx == F-1) begin
            if (n_idx == N-1) begin
              st <= S_IDLE; // step done
            end else begin
              n_idx <= n_idx + 1;
              st    <= S_YUPD;
            end
          end else begin
            f_idx <= f_idx + 1;
            st    <= S_FLOOP;
          end
        end

        default: st <= S_IDLE;
      endcase
    end
  end

endmodule

`default_nettype wire
