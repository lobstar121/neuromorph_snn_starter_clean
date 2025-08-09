import os, json
import numpy as np

ART = os.path.join(os.path.dirname(__file__), "artifacts")

def read_hex_q14(path):
    vals = []
    with open(path, "r") as f:
        for ln in f:
            ln = ln.strip()
            if not ln: continue
            x = int(ln, 16)
            if x & (1 << 15):  # sign
                x -= (1 << 16)
            vals.append(np.int16(x))
    return np.array(vals, dtype=np.int16)

def q14_saturate_int16(x32):
    if x32 >  32767: return np.int16( 32767)
    if x32 < -32768: return np.int16(-32768)
    return np.int16(x32 & 0xFFFF) if isinstance(x32, (np.int32, int)) else np.int16(x32)

def simulate_fixed(X_csv="X_events_ref.csv", W_hex="weights.hex", VTH_hex="vth.hex",
                   T=None, F=48, N=96, Q=14, alpha_q14=15474, refrac_steps=2, out_csv="spikes_sw_q14.csv"):
    # 1) load stimulus
    X = np.loadtxt(os.path.join(ART, X_csv), delimiter=",", dtype=int)  # (T,F)
    if X.ndim != 2 or X.shape[1] != F:
        raise SystemExit(f"[ERROR] stimulus shape {X.shape} != (T,{F})")
    if T is None: T = X.shape[0]

    # 2) load weights/thresholds (Q1.14 int16)
    W_flat = read_hex_q14(os.path.join(ART, W_hex))     # length = F*N
    Vth    = read_hex_q14(os.path.join(ART, VTH_hex))   # length = N
    if W_flat.size != F*N: raise SystemExit(f"[ERROR] weights length {W_flat.size} != F*N={F*N}")
    if Vth.size != N:      raise SystemExit(f"[ERROR] vth length {Vth.size} != N={N}")
    W = W_flat.reshape(F, N)  # addr = f*N + n

    # 3) state
    V = np.zeros((N,), dtype=np.int16)      # Q1.14
    refrac = np.zeros((N,), dtype=np.int32)
    spikes = np.zeros((T, N), dtype=np.int32)

    # 4) simulate per step (HW와 동일 식)
    for t in range(T):
        # leak32 = alpha * V  (Q1.14 * Q1.14 -> Q2.28 in 32b)
        leak32 = (np.int32(alpha_q14) * np.int32(V))   # (N,)

        # acc32 = sum_f (event[f]? w[f,n]:0) in 32b (w는 이미 Q1.14)
        acc32 = np.zeros((N,), dtype=np.int32)
        f_active = np.nonzero(X[t])[0]
        if f_active.size > 0:
            # 행 우선: addr=f*N + n -> W[f, n]
            acc32 = np.sum(np.int32(W[f_active, :]), axis=0)

        # signed rounding on leak: ((leak32 + bias) >> Q)
        bias = np.where(leak32 >= 0, (1 << (Q-1)), -(1 << (Q-1))).astype(np.int32)
        leak_q14 = ((leak32 + bias) >> Q).astype(np.int32)  # back to Q1.14 (in 32b)

        # sum & saturate to int16 Q1.14
        sum32 = leak_q14 + acc32   # still 32b Q1.14
        V_next = np.empty((N,), dtype=np.int16)
        for n in range(N):
            V_next[n] = q14_saturate_int16(sum32[n])

        # spike decision with refractory gating
        active = (refrac <= 0)
        fire = np.logical_and(active, np.int32(V_next) >= np.int32(Vth))
        spikes[t, fire] = 1

        # reset & refrac update
        V = V_next.copy()
        V[fire] = np.int16(0)                # v_reset = 0
        refrac[fire] = refrac_steps
        refrac[~fire] = np.maximum(0, refrac[~fire] - 1)

    out_path = os.path.join(ART, out_csv)
    np.savetxt(out_path, spikes, fmt="%d", delimiter=",")
    print(f"[Saved] {out_path}")
    return out_path

if __name__ == "__main__":
    simulate_fixed()
