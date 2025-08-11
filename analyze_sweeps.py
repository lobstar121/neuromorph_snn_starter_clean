# analyze_sweeps.py  (fix: robust CSV reader that skips header if present)
import os, sys, numpy as np

ART = "artifacts"
BASELINE_RATE = 0.05  # target spike rate

def have(p): return os.path.exists(p)

def _needs_skip_first_row(path):
    """Return True if first row looks like a header (non-numeric)."""
    with open(path, "r", encoding="utf-8") as f:
        first = f.readline().strip()
    # 빈 줄이면 스킵
    if not first:
        return True
    # 콤마 기준으로 숫자성 확인
    for tok in first.split(","):
        tok = tok.strip()
        try:
            float(tok)
        except ValueError:
            return True
    return False

def read_csv(path):
    skip = 1 if _needs_skip_first_row(path) else 0
    A = np.loadtxt(path, delimiter=",", dtype=float, ndmin=2, skiprows=skip)
    if A.ndim == 1:  # 한 줄만 있을 때 (N,) → (1,N)
        A = A.reshape(1, -1)
    return A

def read_hex_q14(path):
    xs=[]
    with open(path) as f:
        for ln in f:
            ln=ln.strip()
            if not ln: continue
            x=int(ln,16)
            if x & (1<<15): x -= (1<<16)
            xs.append(np.int16(x))
    return np.array(xs, dtype=np.int16)

def write_hex_q14(path, arr):
    with open(path,"w") as f:
        for v in arr.astype(np.int16):
            x = int(v)
            if x < 0: x = (1<<16) + x
            f.write(f"{x:04x}\n")

def make_vth_shift(base_hex_path, shift, out_path):
    v = read_hex_q14(base_hex_path).astype(np.int32)
    v2 = np.clip(v + int(shift), -32768, 32767).astype(np.int16)
    write_hex_q14(out_path, v2)

def pick_from_grid(grid_csv):
    # header: alpha_q14,vth_shift_LSB,match_ratio,mean_spike_rate
    A = read_csv(grid_csv)
    rows = A[np.isclose(A[:,2], 1.0)]
    if rows.size == 0:
        max_match = np.max(A[:,2])
        rows = A[A[:,2] >= max_match - 1e-12]
    absmin = np.min(np.abs(rows[:,1]))
    rows = rows[np.abs(rows[:,1]) <= absmin + 1e-12]
    idx = np.argmin(np.abs(rows[:,3] - BASELINE_RATE))
    a, s, mr, sr = rows[idx]
    return int(round(a)), int(round(s)), float(mr), float(sr), "grid"

def pick_from_vth(vth_csv, alpha_fixed):
    # header: shift_LSB,match_ratio,mean_spike_rate
    A = read_csv(vth_csv)
    rows = A[np.isclose(A[:,1], 1.0)]
    if rows.size == 0:
        max_match = np.max(A[:,1])
        rows = A[A[:,1] >= max_match - 1e-12]
    absmin = np.min(np.abs(rows[:,0]))
    rows = rows[np.abs(rows[:,0]) <= absmin + 1e-12]
    idx = np.argmin(np.abs(rows[:,2] - BASELINE_RATE))
    s, mr, sr = rows[idx]
    return alpha_fixed, int(round(s)), float(mr), float(sr), "vth"

def pick_from_alpha(alpha_csv):
    # header: ALPHA_Q14,mean_spike_rate
    A = read_csv(alpha_csv)
    idx = np.argmin(np.abs(A[:,1] - BASELINE_RATE))
    a, sr = A[idx]
    return int(round(a)), 0, None, float(sr), "alpha"

def main():
    os.makedirs(ART, exist_ok=True)
    grid_csv = os.path.join(ART, "grid_sweep_results.csv")
    vth_csv  = os.path.join(ART, "vth_sweep_results.csv")
    alpha_csv= os.path.join(ART, "alpha_sweep_hw_rates.csv")
    base_vth = os.path.join(ART, "vth.hex")

    if have(grid_csv):
        choice = pick_from_grid(grid_csv)
    elif have(vth_csv):
        choice = pick_from_vth(vth_csv, alpha_fixed=15474)
    elif have(alpha_csv):
        choice = pick_from_alpha(alpha_csv)
    else:
        print("[analyze] no sweep csv found — nothing to do."); sys.exit(0)

    alpha, shift, match, rate, source = choice
    print(f"[select] source={source}  alpha={alpha}  vth_shift={shift:+d}  "
          f"match={match if match is not None else 'N/A'}  rate={rate:.6f}")

    alpha_txt = os.path.join(ART, "alpha_selected.txt")
    with open(alpha_txt, "w") as f:
        f.write(f"{alpha}\n")
    print(f"[write] {alpha_txt}")

    vth_sel = os.path.join(ART, "vth_selected.hex")
    if shift == 0:
        write_hex_q14(vth_sel, read_hex_q14(base_vth))
        print(f"[write] {vth_sel} (copied from vth.hex)")
    else:
        make_vth_shift(base_vth, shift, vth_sel)
        print(f"[write] {vth_sel} (shift={shift:+d} LSB)")

if __name__ == "__main__":
    main()
