# learn_stdp_sw.py
import os, sys, argparse, numpy as np
from learn_stdp_q14 import STDPParams, STDPState, sat16, round_shift_q, Q

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
    # leak (Q14 * Q14 -> Q28 >> 14)
    leak32 = np.int64(alpha_q14) * np.int64(V.astype(np.int64))
    leak_q14 = round_shift_q(leak32, Q)  # int64

    if np.any(X_row):
        # 활성 채널 가중치 합산
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
