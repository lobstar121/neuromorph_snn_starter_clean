import os, sys, json
import numpy as np
from sim.lif_core import LIFCore

ART = os.path.join(os.path.dirname(__file__), "artifacts")

def run_on(stim_path):
    X = np.loadtxt(stim_path, delimiter=",", dtype=float)   # (T,F)
    W = np.loadtxt(os.path.join(ART, "W_in.csv"), delimiter=",", dtype=float)  # (F,N)
    cfg = json.load(open(os.path.join(ART, "config.json"), "r", encoding="utf-8"))
    p = cfg["params"]
    v_th = float(cfg.get("v_th_mean", p.get("v_th", 1.0)))

    T, F = X.shape
    Fw, N = W.shape
    assert F == Fw, f"stim F={F} vs weights F={Fw} mismatch"
    print(f"[ReplayOn] stim={os.path.basename(stim_path)}  T={T}, F={F}, N={N}")

    core = LIFCore(
        n_inputs=F, n_neurons=N,
        dt=p.get("dt", 0.002), tau_m=p.get("tau_m", 0.035),
        v_th=v_th, v_reset=p.get("v_reset", 0.0), refrac=p.get("refrac", 0.004),
        w_in_scale=1.0, recurrent=False, seed=0
    )
    core.W_in = W.astype(np.float32)

    spikes, _ = core.forward(X)
    spikes = spikes.astype(np.int32)  # <-- 확실히 정수화
    total = int(spikes.sum())
    per_neuron = spikes.sum(axis=0).astype(np.int32)  # (N,)

    nz_idx = np.where(per_neuron > 0)[0]
    print(f"[ReplayOn] total spikes = {total}  |  active neurons = {len(nz_idx)}")

    if len(nz_idx) == 0:
        print("[ReplayOn] No neurons fired.")
    else:
        # 상위 20개만 깔끔히 출력
        top = nz_idx[np.argsort(-per_neuron[nz_idx])]  # counts 내림차순
        top_k = top[:20]
        pairs = " ".join([f"{i}:{int(per_neuron[i])}" for i in top_k])
        print("[ReplayOn] top-active neurons (id:count):", pairs)

    # save csv
    base = os.path.splitext(os.path.basename(stim_path))[0]
    out_path = os.path.join(ART, f"spikes_sw_{base}.csv")
    np.savetxt(out_path, spikes, fmt="%d", delimiter=",")
    print(f"[Saved] {out_path}")

if __name__ == "__main__":
    if len(sys.argv) < 1:
        print("Usage: python replay_on_stimulus.py <path/to/stim_csv>")
        sys.exit(1)
    if len(sys.argv) == 1:
        print("Usage: python replay_on_stimulus.py <path/to/stim_csv>")
        sys.exit(1)
    run_on(sys.argv[1])
