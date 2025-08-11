# learn_stdp_q14.py
import numpy as np

Q = 14
ONE_Q14 = 1 << Q
I16_MIN, I16_MAX = -32768, 32767

def sat16(x):
    x = np.asarray(x, dtype=np.int64)
    x = np.clip(x, I16_MIN, I16_MAX)
    return x.astype(np.int16)

def round_shift_q(s32, q=Q):
    s32 = np.asarray(s32, dtype=np.int64)
    bias = np.where(s32 >= 0, 1 << (q - 1), -(1 << (q - 1)))
    return ((s32 + bias) >> q).astype(np.int64)

class STDPParams:
    def __init__(
        self,
        eta=8,             # 학습률 계수(정수)
        eta_shift=12,      # 학습률 시프트(η / 2^eta_shift)
        lambda_x=15565,    # 추적자 감쇠(≈0.95 * 2^14)
        lambda_y=15565,    # 추적자 감쇠(≈0.95 * 2^14)
        b_pre=1024,        # pre 스파이크 시 x_pre bump (≈0.0625)
        b_post=1024,       # post 스파이크 시 y_post bump
        wmin=-16384,       # 가중치 하한(≈-1.0)
        wmax=+16384,       # 가중치 상한(≈+1.0)
        enable_pre=False,  # pre 경로 사용
        enable_post=True   # post 경로 사용(권장: 먼저 이거만)
    ):
        self.eta = int(eta)
        self.eta_shift = int(eta_shift)
        self.lambda_x = int(lambda_x)
        self.lambda_y = int(lambda_y)
        self.b_pre = int(b_pre)
        self.b_post = int(b_post)
        self.wmin = int(wmin)
        self.wmax = int(wmax)
        self.enable_pre = bool(enable_pre)
        self.enable_post = bool(enable_post)

class STDPState:
    def __init__(self, F, N):
        self.x_pre = np.zeros((F,), dtype=np.int16)  # 채널별 추적자
        self.y_post = np.zeros((N,), dtype=np.int16) # 뉴런별 추적자

def decay_tracker(x_q14, lam_q14):
    # x <- (lam * x) >> Q (라운딩)
    s32 = np.int64(lam_q14) * np.int64(x_q14.astype(np.int64))
    return sat16(round_shift_q(s32, Q))

def step_stdp(pre_vec, post_vec, W, params: STDPParams, state: STDPState):
    """
    pre_vec: (F,) {0,1}
    post_vec: (N,) {0,1}
    W: (F,N) int16 (in-place 업데이트)
    """
    F, N = W.shape
    pre_vec = pre_vec.astype(np.int16)
    post_vec = post_vec.astype(np.int16)

    # 1) decay
    state.x_pre = decay_tracker(state.x_pre, params.lambda_x)
    state.y_post = decay_tracker(state.y_post, params.lambda_y)

    # 2) bump on spikes
    if np.any(pre_vec):
        state.x_pre = sat16(state.x_pre + (pre_vec * params.b_pre))
    if np.any(post_vec):
        state.y_post = sat16(state.y_post + (post_vec * params.b_post))

    # 3) weight updates
    dW = np.zeros_like(W, dtype=np.int64)

    if params.enable_post and np.any(post_vec):
        # post 스파이크 뉴런들만 열 방향으로 업데이트
        idx_n = np.where(post_vec != 0)[0]
        # Δw(f,n) -= (η * x_pre[f]) >> eta_shift
        # (열별로 동일한 x_pre 적용)
        delta_f = -((params.eta * np.int64(state.x_pre)) >> params.eta_shift)  # (F,)
        for n in idx_n:
            dW[:, n] += delta_f

    if params.enable_pre and np.any(pre_vec):
        # pre 스파이크 채널들만 행 방향으로 업데이트
        idx_f = np.where(pre_vec != 0)[0]
        # Δw(f,n) += (η * y_post[n]) >> eta_shift
        delta_n = ((params.eta * np.int64(state.y_post)) >> params.eta_shift)   # (N,)
        for f in idx_f:
            dW[f, :] += delta_n

    if np.any(dW):
        W[:] = sat16(np.clip(np.int64(W) + dW, params.wmin, params.wmax))
