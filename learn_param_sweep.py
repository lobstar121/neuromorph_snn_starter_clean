# learn_param_sweep.py
import os, json, itertools, subprocess, hashlib
import numpy as np

ART = "artifacts"

def run(cmd):
    print("[RUN]", " ".join(cmd))
    r = subprocess.run(cmd, capture_output=True, text=True)
    if r.returncode != 0:
        print(r.stdout); print(r.stderr)
        raise SystemExit(r.returncode)
    return r

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
            x = int(v);  x = (1<<16)+x if x<0 else x
            f.write(f"{x:04x}\n")

def spike_rate(csv):
    S = np.loadtxt(csv, delimiter=",", dtype=int)
    return float(S.mean())

def clip_frac(W_before, W_after):
    # int16 범위 ±32768 근처 포화율 추정
    Wa = W_after.astype(np.int32)
    return float(((Wa<=-32768) | (Wa>=32767)).mean())

def mean_abs_dw(W0, W1):
    return float(np.mean(np.abs(W1.astype(np.int32)-W0.astype(np.int32))))

def md5(s: str) -> str:
    return hashlib.md5(s.encode()).hexdigest()[:8]

def main():
    os.makedirs(ART, exist_ok=True)

    # --- 탐색공간(필요 시 수정) ---
    alphas     = [15360, 15474, 15600]
    vth_shifts = [0, +1, -1]
    etas       = [4, 8]
    eta_shifts = [11, 12]
    lambdas    = [15500, 15565]   # lambda_x=lambda_y 동일 가정
    b_pre_list  = [1024]
    b_post_list = [1024]
    T = 64
    F, N = 48, 96
    # ----------------------------

    base_weights = os.path.join(ART, "weights.hex")
    base_vth     = os.path.join(ART, "vth.hex")
    X_csv        = os.path.join(ART, "X_events_ref.csv")

    if not os.path.exists(base_weights) or not os.path.exists(base_vth) or not os.path.exists(X_csv):
        raise SystemExit("[ERR] artifacts/weights.hex , artifacts/vth.hex , artifacts/X_events_ref.csv 가 필요합니다.")

    W0 = read_hex_q14(base_weights)

    rows = []
    best = None  # (loss, row_dict, best_weight_hex)

    for a, s, eta, es, lam, bpre, bpost in itertools.product(
            alphas, vth_shifts, etas, eta_shifts, lambdas, b_pre_list, b_post_list):

        # vth shift 적용 파일명
        if s == 0:
            vth_hex = base_vth
            vth_tag = "v0"
        else:
            v = read_hex_q14(base_vth)
            v2 = np.clip(v.astype(np.int32)+int(s), -32768, 32767).astype(np.int16)
            tag = f"v{s:+d}".replace("+","p")
            vth_hex = os.path.join(ART, f"vth_shift_{tag}.hex")
            write_hex_q14(vth_hex, v2)
            vth_tag = tag

        tag = f"a{a}_v{vth_tag}_e{eta}_es{es}_l{lam}_bp{bpre}_bo{bpost}_{md5(str((a,s,eta,es,lam,bpre,bpost)))}"
        out_w = os.path.join(ART, f"weights_learned_{tag}.hex")
        out_s = os.path.join(ART, f"spikes_sw_learn_{tag}.csv")

        # 학습 실행 (SW)
        cmd = [
            "python", "learn_stdp_sw.py",
            "--in", X_csv,
            "--weights-in", base_weights,
            "--weights-out", out_w,
            "--vth", vth_hex,
            "--F", str(F), "--N", str(N), "--T", str(T),
            "--alpha", str(a), "--refrac", "2", "--thresh-mode", "ge",
            "--eta", str(eta), "--eta-shift", str(es),
            "--lambda-x", str(lam), "--lambda-y", str(lam),
            "--b-pre", str(bpre), "--b-post", str(bpost),
            "--save-spikes", out_s
        ]
        run(cmd)

        # 지표 계산
        W1 = read_hex_q14(out_w)
        sr  = spike_rate(out_s)
        mdw = mean_abs_dw(W0, W1)
        cf  = clip_frac(W0, W1)

        # 간단 loss: (스파이크율이 너무 높거나 낮지 않게) + (가중치 변화 과도 억제) + (클리핑 억제)
        # 필요 시 가중치 조절
        loss = (abs(sr-0.05)*5.0) + (mdw/3000.0) + (cf*3.0)

        row = dict(alpha=a, vth_shift=s, eta=eta, eta_shift=es,
                   lambda_x=lam, lambda_y=lam, b_pre=bpre, b_post=bpost,
                   T=T, spike_rate=sr, mean_abs_dW=mdw, clip_frac=cf, loss=loss,
                   weights_hex=os.path.basename(out_w), spikes_csv=os.path.basename(out_s))
        rows.append(row)

        if (best is None) or (loss < best[0]):
            best = (loss, row, out_w)

        print(f"[GRID] {tag}  SR={sr:.4f}  |ΔW|={mdw:.1f}  clip={cf:.3f}  loss={loss:.3f}")

    # CSV 저장
    import csv
    grid_csv = os.path.join(ART, "learn_grid.csv")
    with open(grid_csv, "w", newline="") as f:
        wr = csv.DictWriter(f, fieldnames=list(rows[0].keys()))
        wr.writeheader()
        wr.writerows(rows)
    print(f"[LEARN-GRID] saved {grid_csv} ({len(rows)} rows)")

    # 최적 선택 저장
    assert best is not None
    _, best_row, best_w_path = best
    sel_json = os.path.join(ART, "learn_selected.json")
    with open(sel_json, "w") as f:
        json.dump(best_row, f, indent=2)
    print(f"[LEARN-SELECT] {sel_json}")

    # 고정 이름으로 weights 복사
    dst = os.path.join(ART, "weights_learned_best.hex")
    write_hex_q14(dst, read_hex_q14(best_w_path))
    print(f"[LEARN-BEST] wrote {dst}")

if __name__ == "__main__":
    main()
