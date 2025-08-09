import os
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler

from sim.dataset_audio import build_event_dataset
from sim.train_eval import spike_count_features, train_readout, eval_readout
from sim.stdp_core import LIFCoreSTDP

def ensure_spiking(core, X_ev, min_total_spikes):
    """
    스파이크가 안 나오면 v_th 낮추고 i_bias 올려서 최소 발화를 보장.
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
            core.v_th *= 0.90            # 임계 10% 낮춤
            core.i_bias += 0.01          # 바이어스 +0.01
        trials += 1
    return total

def main():
    base_dir = os.path.join(os.path.dirname(__file__), "data")

    # 이벤트 밀도 ↑ (학습 가동을 위해 적당히 풍부하게)
    # ON/OFF 이벤트 사용 중이면 0.10~0.12 권장
    X_ev, y, meta = build_event_dataset(base_dir, n_per_class=30, n_mels=24, hop=128, thresh=0.10)
    B, T, F = X_ev.shape
    print(f"[Data] B={B}, T={T}, F={F}")

    core = LIFCoreSTDP(
        n_inputs=F, n_neurons=96,
        dt=0.002, tau_m=0.035, v_th=1.0, v_reset=0.0, refrac=0.004,
        w_in_scale=0.75, wmin=-0.9, wmax=0.9,        # 입력 세게(초기 가동)
        tau_pre=0.030, tau_post=0.030,
        A_plus=0.008, A_minus=0.0075,               # STDP 조금 강하게
        i_bias=0.02,                                 # 초기 바이어스
        target_rate=0.06, eta_homeo=0.007, vth_min=0.5, vth_max=2.0,
        seed=0
    )

    # --- 오토킥: 최소 발화량 보장 ---
    kicked = ensure_spiking(core, X_ev, min_total_spikes=int(B*T*0.2))  # 전체 타임의 20% 정도
    print(f"[Priming] ensured total spikes = {int(kicked)}")

    # --- 본 학습 EPOCH ---
    order = np.arange(B)
    EPOCHS = 4
    for epoch in range(EPOCHS):
        np.random.shuffle(order)
        epoch_spikes = 0
        for i in order:
            sp = core.run_sequence(X_ev[i], learn=True)
            epoch_spikes += sp.sum()

        # 학습 중에는 점차 바이어스 ↓ (자립 유도)
        core.i_bias = max(0.0, core.i_bias - 0.005)

        print(f"[Train] epoch {epoch+1}/{EPOCHS} spikes={int(epoch_spikes)} "
              f"v_th(mean)={core.v_th.mean():.3f}  i_bias={core.i_bias:.3f}  W_in std={core.W_in.std():.3f}")

        # 여전히 너무 적으면 한 번 더 보정
        if epoch_spikes < B*T*0.02:   # 너무 적을 때만
            core.v_th *= 0.95
            core.i_bias += 0.005
            print(f"[AutoTune] boost -> v_th(mean)={core.v_th.mean():.3f}  i_bias={core.i_bias:.3f}")

    # --- 평가 (학습 OFF) ---
    spikes_all = []
    for i in range(B):
        sp = core.run_sequence(X_ev[i], learn=False)
        spikes_all.append(sp)
    spikes_all = np.stack(spikes_all, axis=0)
    mean_rate = spikes_all.mean()
    print("[Diag] mean spike rate =", mean_rate)

    X_counts = spike_count_features(spikes_all)
    Xtr, Xte, ytr, yte = train_test_split(X_counts, y, test_size=0.3, stratify=y, random_state=0)
    scaler = StandardScaler().fit(Xtr)
    Xtr = scaler.transform(Xtr)
    Xte = scaler.transform(Xte)

    clf = train_readout(Xtr, ytr)
    acc, cm = eval_readout(clf, Xte, yte)
    print(f"[Eval] acc={acc:.3f}\nConfusion Matrix:\n{cm}")

if __name__ == "__main__":
    main()
