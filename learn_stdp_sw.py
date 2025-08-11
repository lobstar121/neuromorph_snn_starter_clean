# learn_stdp_sw.py
import os, sys, argparse
import numpy as np
from learn_stdp_q14 import STDPParams, STDPState, sat16, round_shift_q, Q as Q_CONST

# --- 유틸 -------------------------------------------------------------

def _q14_round_div(x32: np.ndarray | int, qbits: int) -> np.ndarray:
    """Qx 고정소수점용 부호-반올림 쉬프트 (bias: ±2^(qbits-1))."""
    x64 = np.asarray(x32, dtype=np.int64)
    sgn = np.sign(x64)
    bias = (1 << (qbits - 1)) * sgn
    return ((x64 + bias) >> qbits).astype(np.int64)

def _clip_int16(arr):
    return np.clip(arr, -32768, 32767).astype(np.int16)

def _to_dict(obj):
    """dataclass/Namespace -> dict 로 안전 변환."""
    if isinstance(obj, dict):
        return obj
    try:
        return vars(obj)
    except TypeError:
        return obj.__dict__

def _get_state_arrays(state, F, N):
    """
    state가 dict 이든 STDPState 같은 객체이든 x(F,), y(N,) 배열을 꺼내고,
    없으면 0으로 초기화해서 돌려줌. 또한, 호출자가 넘긴 state에 다시 반영할 수 있게
    setter 함수도 함께 돌려줌.
    """
    # dict 케이스
    if isinstance(state, dict):
        if 'x' not in state or state['x'] is None:
            state['x'] = np.zeros((F,), dtype=np.int64)
        if 'y' not in state or state['y'] is None:
            state['y'] = np.zeros((N,), dtype=np.int64)
        def _setter(x_new, y_new):
            state['x'] = x_new
            state['y'] = y_new
        return state['x'], state['y'], _setter

    # 객체 케이스 (예: STDPState)
    x = getattr(state, 'x', None)
    y = getattr(state, 'y', None)
    if x is None:
        x = np.zeros((F,), dtype=np.int64)
    if y is None:
        y = np.zeros((N,), dtype=np.int64)
    def _setter(x_new, y_new):
        setattr(state, 'x', x_new)
        setattr(state, 'y', y_new)
    return x, y, _setter

# --- STDP 한 스텝 -----------------------------------------------------

def step_stdp(pre_bits: np.ndarray,
              post_bits: np.ndarray,
              W: np.ndarray,
              params,
              state):
    """
    pre_bits : shape (F,)  0/1
    post_bits: shape (N,)  0/1
    W        : shape (F,N) np.int16 (Q14 가중치)
    params   : dict 또는 STDPParams
        keys: Q(옵션,기본14), eta, eta_shift, lambda_x, lambda_y, b_pre, b_post
    state    : dict 또는 STDPState (x: (F,), y: (N,))  Q14 스케일 추적 변수
    """
    # 파라미터 통일
    p = _to_dict(params)
    qbits      = int(p.get('Q', 14))
    eta        = int(p['eta'])
    eta_shift  = int(p['eta_shift'])
    lam_x      = int(p['lambda_x'])
    lam_y      = int(p['lambda_y'])
    b_pre      = int(p['b_pre'])
    b_post     = int(p['b_post'])

    F, N = W.shape
    # 상태 배열 확보 (+ setter)
    x, y, set_state = _get_state_arrays(state, F, N)  # int64, Q14 해석

    # 1) 지수 감쇠
    x[:] = _q14_round_div(lam_x * x, qbits)
    y[:] = _q14_round_div(lam_y * y, qbits)

    # 2) 스파이크 시 추적 증가(Q14)
    if np.any(pre_bits):
        x[:] += (pre_bits.astype(np.int64) * b_pre)
    if np.any(post_bits):
        y[:] += (post_bits.astype(np.int64) * b_post)

    # 3) STDP ΔW = η * ( pre ⊗ y  −  x ⊗ post )  (모든 항 Q14)
    pre_outer_y  = np.outer(pre_bits.astype(np.int64), y)          # (F,N) Q14
    x_outer_post = np.outer(x,                 post_bits.astype(np.int64))  # (F,N) Q14
    dW_q14 = pre_outer_y - x_outer_post                               # (F,N) Q14

    # η 적용 및 스케일 다운
    dW_scaled = (eta * dW_q14) >> eta_shift                           # (F,N) 정수(Q14 해석)

    # 4) 가중치 갱신 + int16 클리핑
    W[:] = _clip_int16(W.astype(np.int64) + dW_scaled)

    # 상태 반영
    set_state(x, y)

# --- 전방 시뮬 --------------------------------------------------------

ART = os.path.join(os.path.dirname(__file__), "artifacts")

def read_hex_q14(path, expect=None):
    xs=[]
    with open(path) as f:
        for ln in f:
            ln=ln.strip()
            if not ln: continue
            x=int(ln,16)
            if x & (1<<15): x -= (1<<16)
            xs.append(np.int16(x))
    arr = np.array(xs, dtype=np.int16)
    if expect is not None and arr.size != expect:
        raise SystemExit(f"[ERR] size mismatch: {path} has {arr.size}, expect {expect}")
    return arr

def write_hex_q14(path, arr):
    with open(path,"w") as f:
        for v in arr.astype(np.int16):
            x = int(v)
            if x < 0: x = (1<<16) + x
            f.write(f"{x:04x}\n")

def forward_step_q14(V, refr, X_row, W, Vth, alpha_q14, refrac_steps, thresh_ge=True):
    """
    V: (N,) int16, refr: (N,) int32
    X_row: (F,) {0,1}, W: (F,N) int16, Vth: (N,) int16
    """
    F, N = W.shape
    # leak (Q14 * Q14 -> Q28 >> Q)
    leak32 = np.int64(alpha_q14) * np.int64(V.astype(np.int64))
    leak_q14 = round_shift_q(leak32, Q_CONST)  # int64

    if np.any(X_row):
        idx = np.where(X_row != 0)[0]
        acc32 = np.sum(np.int64(W[idx, :]), axis=0)  # (N,)
    else:
        acc32 = np.zeros((N,), dtype=np.int64)

    s32 = leak_q14 + acc32                    # (N,) int64
    Vn  = sat16(s32)                          # 다음 막전위
    active = (refr <= 0)
    if thresh_ge:
        fire = np.logical_and(active, np.int64(Vn) >= np.int64(Vth))
    else:
        fire = np.logical_and(active, np.int64(Vn) >  np.int64(Vth))

    # 상태 업데이트
    V[:] = Vn
    V[fire] = np.int16(0)
    refr[fire] = refrac_steps
    refr[~fire] = np.maximum(0, refr[~fire] - 1)

    return fire.astype(np.int16)  # (N,)

# --- 엔트리포인트 -----------------------------------------------------

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--in", dest="csv_in", required=True, help="stim CSV (e.g., artifacts/X_events_ref.csv)")
    ap.add_argument("--weights-in", required=True, help="weights.hex")
    ap.add_argument("--weights-out", required=True, help="output learned weights hex")
    ap.add_argument("--vth", default="artifacts/vth.hex")
    ap.add_argument("--F", type=int, default=48)
    ap.add_argument("--N", type=int, default=96)
    ap.add_argument("--T", type=int, default=0, help="0이면 CSV 길이")
    ap.add_argument("--alpha", type=int, default=15474)
    ap.add_argument("--refrac", type=int, default=2)
    ap.add_argument("--thresh-mode", choices=["ge","gt"], default="ge")

    # STDP params
    ap.add_argument("--eta", type=int, default=8)
    ap.add_argument("--eta-shift", type=int, default=12)
    ap.add_argument("--lambda-x", type=int, default=15565)
    ap.add_argument("--lambda-y", type=int, default=15565)
    ap.add_argument("--b-pre", type=int, default=1024)
    ap.add_argument("--b-post", type=int, default=1024)
    ap.add_argument("--wmin", type=int, default=-16384)
    ap.add_argument("--wmax", type=int, default=+16384)
    ap.add_argument("--enable-pre", action="store_true")
    ap.add_argument("--disable-post", action="store_true")
    ap.add_argument("--save-spikes", default="", help="옵션: SW 스파이크 CSV 저장 경로")

    args = ap.parse_args()

    X = np.loadtxt(args.csv_in, delimiter=",", dtype=int)
    if X.ndim != 2 or X.shape[1] != args.F:
        raise SystemExit(f"[ERR] stim shape {X.shape} != (T,{args.F})")
    T = args.T if args.T and args.T > 0 else X.shape[0]

    Wf = read_hex_q14(args.weights_in, expect=args.F*args.N)
    W  = Wf.reshape(args.F, args.N).copy()
    Vth= read_hex_q14(args.vth, expect=args.N)

    V = np.zeros((args.N,), dtype=np.int16)
    refr = np.zeros((args.N,), dtype=np.int32)
    spikes = np.zeros((T, args.N), dtype=int)

    params = STDPParams(
        eta=args.eta, eta_shift=args.eta_shift,
        lambda_x=args.lambda_x, lambda_y=args.lambda_y,
        b_pre=args.b_pre, b_post=args.b_post,
        wmin=args.wmin, wmax=args.wmax,
        enable_pre=args.enable_pre,
        enable_post=(not args.disable_post)
    )
    state = STDPState(args.F, args.N)

    ge = (args.thresh_mode == "ge")

    for t in range(T):
        pre = X[t, :].astype(np.int16)   # (F,)
        post = forward_step_q14(V, refr, pre, W, Vth, args.alpha, args.refrac, thresh_ge=ge)
        spikes[t, :] = post

        # STDP 한 스텝
        step_stdp(pre, post, W, params, state)

    # 결과 저장
    write_hex_q14(args.weights_out, W.reshape(-1))
    if args.save_spikes:
        np.savetxt(args.save_spikes, spikes, fmt="%d", delimiter=",")
    print(f"[LEARN] wrote {args.weights_out} | spikes saved={bool(args.save_spikes)}")

if __name__ == "__main__":
    main()
