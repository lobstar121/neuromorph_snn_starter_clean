import os
import numpy as np
import matplotlib.pyplot as plt

ART = os.path.join(os.path.dirname(__file__), "artifacts")

def main():
    diff_path = os.path.join(ART, "diff_mask.csv")
    if not os.path.exists(diff_path):
        raise SystemExit("artifacts/diff_mask.csv 가 없습니다. 먼저 compare_hw_sw.py를 실행하세요.")

    diff = np.loadtxt(diff_path, delimiter=",", dtype=int)  # (T, N), 0=일치, 1=불일치
    if diff.ndim != 2:
        raise SystemExit(f"diff_mask.csv shape 문제가 있음: {diff.shape}")

    T, N = diff.shape
    print(f"[Viz] diff_mask shape: T={T}, N={N}")

    # 1) 타임스텝/뉴런별 불일치율
    per_t = diff.mean(axis=1)  # (T,)
    per_n = diff.mean(axis=0)  # (N,)
    worst_t = int(np.argmax(per_t))
    worst_n = int(np.argmax(per_n))
    print(f"[Viz] worst timestep: t={worst_t}, mismatch rate={per_t[worst_t]:.4f}")
    print(f"[Viz] worst neuron  : n={worst_n}, mismatch rate={per_n[worst_n]:.4f}")

    # 2) 히트맵 저장 (시간 x 뉴런)
    os.makedirs(ART, exist_ok=True)
    plt.figure(figsize=(10, 4))
    plt.imshow(diff.T, aspect="auto", interpolation="nearest")  # 색상은 기본값(지정 안 함)
    plt.xlabel("time step (t)")
    plt.ylabel("neuron id (n)")
    plt.title("HW vs SW mismatch (1=mismatch, 0=match)")
    plt.colorbar()
    out_heat = os.path.join(ART, "diff_heatmap.png")
    plt.tight_layout()
    plt.savefig(out_heat, dpi=150)
    plt.close()
    print(f"[Saved] {out_heat}")

    # 3) 요약 CSV
    np.savetxt(os.path.join(ART, "mismatch_per_t.csv"), per_t, delimiter=",")
    np.savetxt(os.path.join(ART, "mismatch_per_neuron.csv"), per_n, delimiter=",")
    print("[Saved] artifacts/mismatch_per_t.csv")
    print("[Saved] artifacts/mismatch_per_neuron.csv")

    # 4) 상위 구간/뉴런 리포트
    K = min(10, T)
    top_t_idx = np.argsort(-per_t)[:K]
    top_n_idx = np.argsort(-per_n)[:K]
    report_lines = []
    report_lines.append(f"# Mismatch Summary\n")
    report_lines.append(f"Match ratio overall = {1.0 - diff.mean():.6f}\n")
    report_lines.append(f"Top-{K} timesteps (t, rate):\n")
    for t in top_t_idx:
        report_lines.append(f"  t={int(t):3d}  rate={per_t[t]:.6f}")
    report_lines.append(f"\nTop-{K} neurons (n, rate):\n")
    for n in top_n_idx:
        report_lines.append(f"  n={int(n):3d}  rate={per_n[n]:.6f}")
    out_report = os.path.join(ART, "mismatch_report.txt")
    with open(out_report, "w", encoding="utf-8") as f:
        f.write("\n".join(report_lines))
    print(f"[Saved] {out_report}")

if __name__ == "__main__":
    main()
