import os, sys, numpy as np

def load(path):
    return np.loadtxt(path, delimiter=",", dtype=int)

def report(spikes_path, prefix_out):
    S = load(spikes_path)  # (T,N)
    T, N = S.shape
    per_t = S.mean(axis=1)          # 각 시점 평균 발화율
    per_n = S.mean(axis=0)          # 각 뉴런 평균 발화율
    total = S.sum()
    mean_rate = S.mean()            # time-neuron 평균
    active_neurons = (per_n>0).sum()

    out_dir = os.path.dirname(spikes_path)
    np.savetxt(os.path.join(out_dir, f"{prefix_out}_per_t.csv"), per_t, fmt="%.6f", delimiter=",")
    np.savetxt(os.path.join(out_dir, f"{prefix_out}_per_n.csv"), per_n, fmt="%.6f", delimiter=",")
    stats_path = os.path.join(out_dir, f"{prefix_out}_stats.txt")
    with open(stats_path, "w") as f:
        f.write(f"path={os.path.basename(spikes_path)}\n")
        f.write(f"T={T} N={N}\n")
        f.write(f"total_spikes={int(total)}\n")
        f.write(f"mean_rate={mean_rate:.6f}\n")
        f.write(f"active_neurons={int(active_neurons)} / {N}\n")
        top = np.argsort(-per_n)[:10]
        low = np.argsort(per_n)[:10]
        f.write("top10_neurons(id:rate)=" + " ".join([f"{i}:{per_n[i]:.4f}" for i in top]) + "\n")
        f.write("low10_neurons(id:rate)=" + " ".join([f"{i}:{per_n[i]:.4f}" for i in low]) + "\n")
    print(f"[Report] saved {prefix_out}_per_t.csv, {prefix_out}_per_n.csv, {prefix_out}_stats.txt")

if __name__=="__main__":
    if len(sys.argv)<3:
        print("Usage: python spike_report.py <spikes.csv> <prefix_out>")
        sys.exit(1)
    report(sys.argv[1], sys.argv[2])
