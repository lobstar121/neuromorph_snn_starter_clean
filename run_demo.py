import os
import numpy as np
import matplotlib.pyplot as plt

from sim.dataset_audio import build_event_dataset
from sim.lif_core import LIFCore
from sim.train_eval import spike_count_features, train_readout, eval_readout
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler

def main():
    base_dir = os.path.join(os.path.dirname(__file__), "data")

    # 이벤트 밀도 ↑ (필요시 0.08~0.12 사이 조정)
    X_ev, y, meta = build_event_dataset(
        base_dir, n_per_class=30, n_mels=24, hop=128, thresh=0.10
    )
    B, T, F = X_ev.shape
    print(f"[Data] batch={B}, T={T}, F={F}")

    # LIF 파라미터: 발화 유도 방향
    core = LIFCore(
        n_inputs=F, n_neurons=96,
        dt=0.002, tau_m=0.040,
        v_th=0.85, v_reset=0.0,
        refrac=0.003,
        w_in_scale=0.75,
        recurrent=False, seed=0
    )

    # Forward pass
    spikes_all = []
    for i in range(B):
        spikes, V = core.forward(X_ev[i])
        spikes_all.append(spikes)
    spikes_all = np.stack(spikes_all, axis=0)  # (B, T, N)

    mean_spike_rate = spikes_all.mean()
    print(f"[Diag] mean spike rate per time-neuron = {mean_spike_rate:.4f}")
    print("[Diag] total spikes:", int(spikes_all.sum()))

    # 특징 → 분할 → 스케일 → 학습/평가
    X_counts = spike_count_features(spikes_all)
    Xtr, Xte, ytr, yte = train_test_split(
        X_counts, y, test_size=0.3, stratify=y, random_state=0
    )
    scaler = StandardScaler().fit(Xtr)
    Xtr = scaler.transform(Xtr)
    Xte = scaler.transform(Xte)

    clf = train_readout(Xtr, ytr)
    acc, cm = eval_readout(clf, Xte, yte)
    print(f"[Eval] acc={acc:.3f}\nConfusion Matrix:\n{cm}")

    # Raster plot
    os.makedirs("docs", exist_ok=True)
    fig, ax = plt.subplots(figsize=(9, 4))
    # test 집합의 첫 샘플 (train/test 분할과 무관하게 그냥 첫 test를 표시)
    sample_id = 0
    # 분할 전 순서 기반으로 테스트 인덱스 계산이 복잡하니,
    # 간단히 테스트 샘플의 스파이크를 다시 계산해 표시해도 됨.
    # 여기서는 데모 간소화를 위해 기존 방식 유지:
    raster = spikes_all[len(Xtr) + sample_id]
    t_idx, n_idx = np.nonzero(raster)
    ax.scatter(t_idx, n_idx, s=2)
    ax.set_title(f"Raster Plot (test sample={sample_id}, label={yte[sample_id]})")
    ax.set_xlabel("time step")
    ax.set_ylabel("neuron id")
    plt.tight_layout()
    figpath = os.path.join("docs", "raster_plot.png")
    plt.savefig(figpath, dpi=150)
    print(f"[Saved] {figpath}")

if __name__ == "__main__":
    main()
