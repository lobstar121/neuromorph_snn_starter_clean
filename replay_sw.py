import os
import json
import numpy as np
import pandas as pd
from sim.lif_core import LIFCore

ARTIFACTS_DIR = os.path.join(os.path.dirname(__file__), "artifacts")

def main():
    # Load artifacts
    X_events = pd.read_csv(os.path.join(ARTIFACTS_DIR, "X_events_ref.csv"), header=None).values
    W_in = pd.read_csv(os.path.join(ARTIFACTS_DIR, "W_in.csv"), header=None).values
    with open(os.path.join(ARTIFACTS_DIR, "config.json"), "r") as f:
        config = json.load(f)
    
    T, F = X_events.shape
    N = W_in.shape[1]
    print(f"[ReplaySW] T={T}, F={F}, N={N}")

    # Build LIFCore with saved parameters
    core = LIFCore(
        n_inputs=F,
        n_neurons=N,
        dt=config.get("dt", 0.002),
        tau_m=config.get("tau_m", 0.03),
        v_th=config.get("v_th_mean", 1.0),
        v_reset=config.get("v_reset", 0.0),
        refrac=config.get("refrac", 0.004),
        w_in_scale=1.0,
        recurrent=False
    )
    core.W_in = W_in

    # Run simulation
    spikes, _ = core.forward(X_events)
    np.savetxt(os.path.join(ARTIFACTS_DIR, "spikes_sw.csv"), spikes, fmt="%d", delimiter=",")
    print(f"[Saved] artifacts/spikes_sw.csv")

    # Compare with reference
    ref_path = os.path.join(ARTIFACTS_DIR, "spikes_ref.csv")
    if os.path.exists(ref_path):
        spikes_ref = pd.read_csv(ref_path, header=None).values
        match_ratio = np.mean(spikes == spikes_ref)
        print(f"[ReplaySW] match vs spikes_ref.csv = {match_ratio:.4f}")

if __name__ == "__main__":
    main()
