# probe_mismatch.py
import numpy as np, sys

hw = np.loadtxt("artifacts/spikes_hw.csv", delimiter=",", dtype=int)
sw = np.loadtxt("artifacts/spikes_sw_q14.csv", delimiter=",", dtype=int)

assert hw.shape == sw.shape, f"shape mismatch: {hw.shape} vs {sw.shape}"
T, N = hw.shape
diff = hw != sw
mismatch_idx = np.argwhere(diff)

print(f"[Probe] shape T={T}, N={N}, mismatches={mismatch_idx.shape[0]}")
if mismatch_idx.size:
    # 상위 20개만 보기
    print("[Probe] sample mismatches (t, n, hw, sw):")
    for k, (t, n) in enumerate(mismatch_idx[:20]):
        print(f"  {t:>2}, {n:>2} : {hw[t,n]} vs {sw[t,n]}")
else:
    print("[Probe] perfect match.")

# 집중 관측: 최악 뉴런 79
n_star = 79
if n_star < N:
    where_n = np.argwhere(diff[:, n_star]).flatten()
    print(f"\n[Probe] neuron {n_star}: mismatch timesteps = {where_n.tolist()}")
    # 그 타임스텝들만 한 줄 요약
    for t in where_n:
        print(f"  t={t:>2} : hw={hw[t,n_star]}  sw={sw[t,n_star]}")
else:
    print(f"[Probe] neuron {n_star} out of range (N={N})")
