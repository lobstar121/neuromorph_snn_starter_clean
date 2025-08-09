import os, json, numpy as np
from datetime import datetime
from sim.dataset_audio import build_event_dataset
from sim.stdp_core import LIFCoreSTDP

ART_DIR = os.path.join(os.path.dirname(__file__), "artifacts")
os.makedirs(ART_DIR, exist_ok=True)

def ensure_spiking(core, X_ev, min_total_spikes):
    """
    스파이크가 너무 적으면 v_th를 낮추고 i_bias를 올려 최소 발화를 보장.
    """
    trials = 0
    total = 0
    while total < min_total_spikes and trials < 8:
        total = 0
        for i in range(len(X_ev)):
            sp = core.run_sequence(X_ev[i], learn=True)
            total += sp.sum()
        print(f"[AutoKick] trial={trials} total_spikes={int(total)}  v_th(mean)={core.v_th.mean():.3f}  i_bias={core.i_bias:.3f}")
        if total < min_total_spikes:
            core.v_th *= 0.90
            core.i_bias += 0.01
        trials += 1
    return total

def train_and_dump():
    # --- dataset (성공했던 세팅과 동일) ---
    X_ev, y, meta = build_event_dataset(
        base_dir=os.path.join(os.path.dirname(__file__), "data"),
        n_per_class=30, n_mels=24, hop=128, thresh=0.10
    )
    B, T, F = X_ev.shape
    print(f"[Data] B={B}, T={T}, F={F}")

    # --- core params (성공했던 세팅과 동일) ---
    params = dict(
        n_inputs=F, n_neurons=96,
        dt=0.002, tau_m=0.035, v_th=1.0, v_reset=0.0, refrac=0.004,
        w_in_scale=0.75, wmin=-0.9, wmax=0.9,
        tau_pre=0.030, tau_post=0.030,
        A_plus=0.008, A_minus=0.0075,
        i_bias=0.02,
        target_rate=0.06, eta_homeo=0.007, vth_min=0.5, vth_max=2.0,
        seed=0
    )
    core = LIFCoreSTDP(**params)

    # --- priming ---
    kicked = ensure_spiking(core, X_ev, min_total_spikes=int(B*T*0.2))
    print(f"[Priming] ensured total spikes = {int(kicked)}")

    # --- training epochs ---
    EPOCHS = 4
    order = np.arange(B)
    for epoch in range(EPOCHS):
        np.random.shuffle(order)
        epoch_spikes = 0
        for i in order:
            sp = core.run_sequence(X_ev[i], learn=True)
            epoch_spikes += sp.sum()
        core.i_bias = max(0.0, core.i_bias - 0.005)
        print(f"[Train] epoch {epoch+1}/{EPOCHS} spikes={int(epoch_spikes)} "
              f"v_th(mean)={core.v_th.mean():.3f}  i_bias={core.i_bias:.3f}  W_in std={core.W_in.std():.3f}")

    # --- reference spikes (T x N) ---
    ref_idx = 0
    spikes_ref = core.run_sequence(X_ev[ref_idx], learn=False)
    print(f"[Diag] ref mean spike rate = {spikes_ref.mean():.6f}")

    # --- dump artifacts ---
    # 1) config.json
    config = {
        "timestamp": datetime.utcnow().isoformat() + "Z",
        "dataset": {"B": int(B), "T": int(T), "F": int(F), **meta},
        "params": params,
        "v_th_mean": float(core.v_th.mean())
    }
    with open(os.path.join(ART_DIR, "config.json"), "w", encoding="utf-8") as f:
        json.dump(config, f, indent=2)

    # 2) W_in.csv  (F x N)
    np.savetxt(os.path.join(ART_DIR, "W_in.csv"), core.W_in, delimiter=",")

    # 3) spikes_ref.csv  (T x N)
    np.savetxt(os.path.join(ART_DIR, "spikes_ref.csv"), spikes_ref, fmt="%d", delimiter=",")

    print("[Saved] artifacts/config.json")
    print("[Saved] artifacts/W_in.csv")
    print("[Saved] artifacts/spikes_ref.csv")

if __name__ == "__main__":
    train_and_dump()
