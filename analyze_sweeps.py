# analyze_sweeps.py
import os, sys, numpy as np

ART = "artifacts"
BASELINE_RATE = 0.05  # “너무 과도/과소 발화 방지”용 타깃 값

def have(p): return os.path.exists(p)

def read_csv(path):
    return np.loadtxt(path, delimiter=",", dtype=float, comments="#")

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
    """
    grid_sweep_results.csv
    header: alpha_q14,vth_shift_LSB,match_ratio,mean_spike_rate
    """
    A = read_csv(grid_csv)
    # 1) match==1.0 행 필터
    best = None
    rows = A[np.isclose(A[:,2], 1.0)]
    if rows.size == 0:
        # 2) 최댓값(match) 선택
        max_match = np.max(A[:,2])
        rows = A[A[:,2] >= max_match - 1e-12]

    # 3) |vth_shift| 최소
    absmin = np.min(np.abs(rows[:,1]))
    rows = rows[np.abs(rows[:,1]) <= absmin + 1e-12]

    # 4) spike rate가 BASELINE_RATE에 가장 가까운 것
    idx = np.argmin(np.abs(rows[:,3] - BASELINE_RATE))
    a, s, mr, sr = rows[idx]
    return int(round(a)), int(round(s)), float(mr), float(sr), "grid"

def pick_from_vth(vth_csv, alpha_fixed):
    """
    vth_sweep_results.csv
    header: shift_LSB,match_ratio,mean_spike_rate
    """
    A = read_csv(vth_csv)
    # alpha는 고정, shift만 고름
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
    """
    alpha_sweep_hw_rates.csv
    header: ALPHA_Q14,mean_spike_rate
    (매치 정보는 없으니 베이스라인에 가장 가까운 값)
    """
    A = read_csv(alpha_csv)
    idx = np.argmin(np.abs(A[:,1] - BASELINE_RATE))
    a, sr = A[idx]
    # vth shift는 0으로 둔다
    return int(round(a)), 0, None, float(sr), "alpha"

def main():
    os.makedirs(ART, exist_ok=True)
    grid_csv = os.path.join(ART, "grid_sweep_results.csv")
    vth_csv  = os.path.join(ART, "vth_sweep_results.csv")
    alpha_csv= os.path.join(ART, "alpha_sweep_hw_rates.csv")
    base_vth = os.path.join(ART, "vth.hex")

    choice = None
    if have(grid_csv):
        choice = pick_from_grid(grid_csv)
    elif have(vth_csv):
        # alpha_sweep 결과가 없어도, 기본값 15474를 우선 시도
        choice = pick_from_vth(vth_csv, alpha_fixed=15474)
    elif have(alpha_csv):
        choice = pick_from_alpha(alpha_csv)
    else:
        print("[analyze] no sweep csv found — nothing to do.")
        sys.exit(0)

    alpha, shift, match, rate, source = choice
    print(f"[select] source={source}  alpha={alpha}  vth_shift={shift:+d}  "
          f"match={match if match is not None else 'N/A'}  rate={rate:.6f}")

    # 저장물
    alpha_txt = os.path.join(ART, "alpha_selected.txt")
    with open(alpha_txt, "w") as f:
        f.write(f"{alpha}\n")
    print(f"[write] {alpha_txt}")

    vth_sel = os.path.join(ART, "vth_selected.hex")
    if shift == 0:
        # 원본을 그대로 복사
        write_hex_q14(vth_sel, read_hex_q14(base_vth))
        print(f"[write] {vth_sel} (copied from vth.hex)")
    else:
        make_vth_shift(base_vth, shift, vth_sel)
        print(f"[write] {vth_sel} (shift={shift:+d} LSB)")

if __name__ == "__main__":
    main()
