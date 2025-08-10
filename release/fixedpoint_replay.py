# fixedpoint_replay.py (patched, with CLI options & debug)
import os
import argparse
import numpy as np

ART_DEFAULT = os.path.join(os.path.dirname(__file__), "artifacts")

def read_hex_q14(path):
    vals = []
    with open(path, "r") as f:
        for ln in f:
            s = ln.strip()
            if not s:
                continue
            x = int(s, 16)
            if x & (1 << 15):  # sign bit
                x -= (1 << 16)
            vals.append(np.int16(x))
    return np.array(vals, dtype=np.int16)

def q14_saturate_int16_scalar(x32):
    """Saturate 32-bit int to int16 range"""
    if x32 >  32767: return np.int16( 32767)
    if x32 < -32768: return np.int16(-32768)
    # ensure dtype int16
    return np.int16(x32)

def parse_args():
    p = argparse.ArgumentParser(description="Fixed-point (Q1.14) SW replay to match HW.")
    p.add_argument("--art", default=ART_DEFAULT, help="artifacts directory")
    p.add_argument("--X-csv", default="X_events_ref.csv", help="stimulus CSV filename (under --art)")
    p.add_argument("--W-hex", default="weights.hex", help="weights hex filename (under --art)")
    p.add_argument("--VTH-hex", default="vth.hex", help="threshold hex filename (under --art)")
    p.add_argument("-o","--out", default="spikes_sw_q14.csv", help="output CSV (under --art)")
    p.add_argument("--F", type=int, default=48, help="number of inputs (features)")
    p.add_argument("--N", type=int, default=96, help="number of neurons")
    p.add_argument("--T", type=int, default=None, help="timesteps; default = from stimulus")
    p.add_argument("--Q", type=int, default=14, help="fractional bits (Q1.14)")
    p.add_argument("--alpha-q14", type=int, default=15474, help="leak factor in Q1.14 (e.g., 15474)")
    p.add_argument("--refrac-steps", type=int, default=2, help="refractory steps after spike")
    p.add_argument("--cmp", choices=["ge","gt"], default="ge",
                   help="threshold compare: ge => >= v_th (default), gt => > v_th")
    p.add_argument("--dbg-n", type=int, default=None, help="debug: neuron index to trace")
    p.add_argument("--dbg-t0", type=int, default=None, help="debug: start t (inclusive)")
    p.add_argument("--dbg-t1", type=int, default=None, help="debug: end t (inclusive)")
    return p.parse_args()

def simulate_fixed(art_dir, X_csv, W_hex, VTH_hex,
                   T, F, N, Q, alpha_q14, refrac_steps, cmp_mode, out_csv,
                   dbg_n=None, dbg_t0=None, dbg_t1=None):
    # 1) load stimulus
    X_path = os.path.join(art_dir, X_csv)
    X = np.loadtxt(X_path, delimiter=",", dtype=int)  # (T,F)
    if X.ndim != 2 or X.shape[1] != F:
        raise SystemExit(f"[ERROR] stimulus shape {X.shape} != (T,{F})")
    if T is None:
        T = X.shape[0]
    elif T > X.shape[0]:
        raise SystemExit(f"[ERROR] requested T={T} > stimulus T={X.shape[0]}")

    # 2) load weights/thresholds (Q1.14 int16)
    W_flat = read_hex_q14(os.path.join(art_dir, W_hex))     # length = F*N
    Vth    = read_hex_q14(os.path.join(art_dir, VTH_hex))   # length = N
    if W_flat.size != F*N:
        raise SystemExit(f"[ERROR] weights length {W_flat.size} != F*N={F*N}")
    if Vth.size != N:
        raise SystemExit(f"[ERROR] vth length {Vth.size} != N={N}")
    W = W_flat.reshape(F, N)  # addr = f*N + n

    # 3) state
    V = np.zeros((N,), dtype=np.int16)      # membrane potential in Q1.14
    refrac = np.zeros((N,), dtype=np.int32)
    spikes = np.zeros((T, N), dtype=np.int32)

    # helper for rounding-to-nearest with sign
    bias_pos = np.int32(1 << (Q-1))
    bias_neg = -bias_pos

    def round_q(back32):
        # back32 is int32 Q(1.14)-like
        bias = np.where(back32 >= 0, bias_pos, bias_neg).astype(np.int32)
        return ((back32 + bias) >> Q).astype(np.int32)

    # 4) simulate per step (align with HW)
    for t in range(T):
        # leak32 = alpha * V  (Q1.14 * Q1.14 -> Q2.28 in 32b)
        leak32 = (np.int32(alpha_q14) * np.int32(V))   # (N,)

        # acc32 = sum_f (event[f]? w[f,n]:0) in 32b (w는 이미 Q1.14)
        f_active = np.nonzero(X[t])[0]
        if f_active.size > 0:
            acc32 = np.sum(np.int32(W[f_active, :]), axis=0)
        else:
            acc32 = np.zeros((N,), dtype=np.int32)

        # signed rounding on leak: ((leak32 + bias) >> Q)  -> back to Q1.14 in 32b
        leak_q14 = round_q(leak32)

        # sum & saturate to int16 Q1.14
        sum32 = leak_q14 + acc32   # still 32b in Q1.14
        # saturate elementwise to int16
        V_next = np.empty((N,), dtype=np.int16)
        for n in range(N):
            V_next[n] = q14_saturate_int16_scalar(sum32[n])

        # threshold compare
        active = (refrac <= 0)
        if cmp_mode == "ge":
            fire = np.logical_and(active, np.int32(V_next) >= np.int32(Vth))
        else:  # "gt"
            fire = np.logical_and(active, np.int32(V_next) >  np.int32(Vth))
        spikes[t, fire] = 1

        # optional debug
        if (dbg_n is not None) and (dbg_t0 is not None) and (dbg_t1 is not None):
            if dbg_t0 <= t <= dbg_t1:
                n = dbg_n
                # leak_q14[n], acc32[n], sum32[n], V_next[n], spike
                print(f"[SWDBG] t={t} n={n} "
                      f"leak_q14={int(leak_q14[n])} acc32={int(acc32[n])} sum32={int(sum32[n])} "
                      f"V_next={int(V_next[n])} Vth={int(Vth[n])} "
                      f"spike={int(spikes[t, n])}")

        # reset & refrac update (match HW order)
        V = V_next.copy()
        V[fire] = np.int16(0)                # v_reset = 0
        refrac[fire] = refrac_steps
        refrac[~fire] = np.maximum(0, refrac[~fire] - 1)

    out_path = os.path.join(art_dir, out_csv)
    np.savetxt(out_path, spikes, fmt="%d", delimiter=",")
    total = int(spikes.sum())
    active_neurons = int((spikes.sum(axis=0) > 0).sum())
    mean_rate = float(spikes.mean())
    print(f"[Saved] {out_path}")
    print(f"[Diag] total spikes={total} | active neurons={active_neurons} | mean spike rate={mean_rate:.6f}")
    return out_path

if __name__ == "__main__":
    args = parse_args()
    simulate_fixed(
        art_dir=args.art,
        X_csv=args.X_csv,
        W_hex=args.W_hex,
        VTH_hex=args.VTH_hex,
        T=args.T,
        F=args.F,
        N=args.N,
        Q=args.Q,
        alpha_q14=args.alpha_q14,
        refrac_steps=args.refrac_steps,
        cmp_mode=args.cmp,
        out_csv=args.out,
        dbg_n=args.dbg_n,
        dbg_t0=args.dbg_t0,
        dbg_t1=args.dbg_t1
    )
