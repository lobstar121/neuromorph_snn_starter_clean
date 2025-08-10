import os, sys
import numpy as np

def load_csv(path):
    try:
        return np.loadtxt(path, delimiter=",", dtype=int)
    except Exception as e:
        raise SystemExit(f"[ERROR] failed to load {path}: {e}")

def main():
    if len(sys.argv) != 3:
        print("Usage: python compare_spikes.py <pred.csv> <gold.csv>")
        print("  ex) python compare_spikes.py artifacts/spikes_hw.csv artifacts/spikes_sw.csv")
        print("  ex) python compare_spikes.py artifacts/spikes_sw.csv artifacts/spikes_ref.csv")
        sys.exit(1)

    pred_path, gold_path = sys.argv[1], sys.argv[2]
    pred = load_csv(pred_path)
    gold = load_csv(gold_path)

    if pred.shape != gold.shape:
        raise SystemExit(f"[ERROR] shape mismatch: {pred.shape} (pred) vs {gold.shape} (gold)")

    diff = (pred != gold)
    match = 1.0 - diff.mean()
    num_diff = int(diff.sum())
    T, N = pred.shape

    print(f"[Compare] {os.path.basename(pred_path)} vs {os.path.basename(gold_path)}")
    print(f"[Compare] shape: T={T}, N={N}")
    print(f"[Compare] match ratio: {match:.6f}  (diff cells = {num_diff})")

    # 저장: diff mask (0=일치, 1=불일치)
    out_mask = os.path.join(os.path.dirname(pred_path), "diff_mask.csv")
    np.savetxt(out_mask, diff.astype(int), fmt="%d", delimiter=",")
    print(f"[Saved] {out_mask}")

    # 보너스: 불일치가 많은 타임스텝/뉴런 상위 몇 개 찍어주기
    per_t = diff.mean(axis=1)
    per_n = diff.mean(axis=0)
    worst_t = int(np.argmax(per_t))
    worst_n = int(np.argmax(per_n))
    print(f"[Compare] worst timestep: t={worst_t} (mismatch rate={per_t[worst_t]:.6f})")
    print(f"[Compare] worst neuron  : n={worst_n} (mismatch rate={per_n[worst_n]:.6f})")

if __name__ == "__main__":
    main()
