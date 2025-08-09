import numpy as np

class LIFCoreSTDP:
    """
    LIF + 입력가중치(STDP) 온라인 학습 코어
    + 홈오스테이시스(뉴런별 목표 발화율로 v_th 자동 조정)
    - Δw = A_plus * pre_trace * post_spike  -  A_minus * post_trace * pre_spike
    """

    def __init__(self, n_inputs, n_neurons,
                 dt=0.002, tau_m=0.030, v_th=1.1, v_reset=0.0, refrac=0.004,
                 w_in_scale=0.5, wmin=-0.8, wmax=0.8,
                 tau_pre=0.020, tau_post=0.020, A_plus=0.004, A_minus=0.0035,
                 i_bias=0.0,
                 # 홈오스테이시스 파라미터
                 target_rate=0.05,    # 시퀀스 평균 목표 발화율 (0~1)
                 eta_homeo=0.005,     # 임계치 적응 속도
                 vth_min=0.5, vth_max=2.0,
                 seed=0):
        rng = np.random.default_rng(seed)
        self.F = n_inputs
        self.N = n_neurons
        self.dt = dt
        self.alpha = np.exp(-dt / tau_m)
        # v_th를 뉴런별 배열로 운용 (홈오스테이시스)
        self.v_th = np.full((n_neurons,), v_th, dtype=np.float32)
        self.v_reset = v_reset
        self.refrac_steps = int(refrac / dt)
        self.i_bias = float(i_bias)

        self.W_in = rng.normal(0, w_in_scale/np.sqrt(n_inputs), size=(n_inputs, n_neurons)).astype(np.float32)

        # STDP traces
        self.pre_trace  = np.zeros((n_inputs,),  dtype=np.float32)
        self.post_trace = np.zeros((n_neurons,), dtype=np.float32)
        self.alpha_pre  = np.exp(-dt / tau_pre)
        self.alpha_post = np.exp(-dt / tau_post)
        self.A_plus  = A_plus
        self.A_minus = A_minus
        self.wmin, self.wmax = wmin, wmax

        # 홈오스테이시스
        self.target_rate = float(target_rate)
        self.eta_homeo   = float(eta_homeo)
        self.vth_min     = float(vth_min)
        self.vth_max     = float(vth_max)

    def step(self, x_t, V_prev, spikes_prev, refrac_cnt):
        # leak
        V_t = self.alpha * V_prev

        # 입력 전류 + 바이어스 (발화 킥스타트용)
        I_in = x_t.astype(np.float32) @ self.W_in + self.i_bias
        V_t += I_in

        # 발화/리셋/불응
        active = refrac_cnt <= 0
        fired  = (V_t >= self.v_th) & active    # per-neuron threshold
        spikes_t = fired.astype(np.uint8)
        V_t[fired] = self.v_reset
        refrac_cnt[fired] = self.refrac_steps
        refrac_cnt[~fired] = np.maximum(0, refrac_cnt[~fired] - 1)

        # --- STDP 업데이트 ---
        self.pre_trace  *= self.alpha_pre
        self.post_trace *= self.alpha_post
        self.pre_trace  += x_t.astype(np.float32)
        self.post_trace += spikes_t.astype(np.float32)

        ltp = np.outer(self.pre_trace, spikes_t).astype(np.float32)
        ltd = np.outer(x_t.astype(np.float32), self.post_trace).astype(np.float32)
        dW = self.A_plus * ltp - self.A_minus * ltd
        self.W_in = np.clip(self.W_in + dW, self.wmin, self.wmax)

        return V_t, spikes_t, refrac_cnt

    def run_sequence(self, X_events, learn=True):
        T, F = X_events.shape
        assert F == self.F
        V = np.zeros((self.N,), dtype=np.float32)
        spikes_prev = np.zeros((self.N,), dtype=np.uint8)
        refrac_cnt = np.zeros((self.N,), dtype=np.int32)
        spikes_seq = np.zeros((T, self.N), dtype=np.uint8)

        # 학습 off면 trace/가중치 갱신 스킵
        A_plus, A_minus = self.A_plus, self.A_minus
        if not learn:
            self.A_plus = self.A_minus = 0.0

        for t in range(T):
            V, sp, refrac_cnt = self.step(X_events[t], V, spikes_prev, refrac_cnt)
            spikes_seq[t] = sp
            spikes_prev = sp

        # 홈오스테이시스 (시퀀스 평균 발화율로 임계 자동 보정)
        if learn:
            rate = spikes_seq.mean(axis=0)  # 각 뉴런별 T 평균
            self.v_th += self.eta_homeo * (rate - self.target_rate)
            self.v_th = np.clip(self.v_th, self.vth_min, self.vth_max)

        # 복구
        self.A_plus, self.A_minus = A_plus, A_minus
        return spikes_seq
