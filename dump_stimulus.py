import os
import numpy as np
from sim.dataset_audio import build_event_dataset

def main():
    base = os.path.dirname(__file__)
    art = os.path.join(base, "artifacts")
    os.makedirs(art, exist_ok=True)

    # dump_artifacts.py에서 쓴 데이터셋 설정과 반드시 동일하게
    X_ev, y, meta = build_event_dataset(
        base_dir=os.path.join(base, "data"),
        n_per_class=30, n_mels=24, hop=128, thresh=0.10
    )
    B, T, F = X_ev.shape
    print(f"[Stim] B={B}, T={T}, F={F}")

    # 참조 샘플 (dump_artifacts.py와 맞춤)
    ref_idx = 0
    X_ref = X_ev[ref_idx].astype(int)  # (T, F) 0/1
    out_path = os.path.join(art, "X_events_ref.csv")
    np.savetxt(out_path, X_ref, fmt="%d", delimiter=",")
    print(f"[Saved] {out_path}")

if __name__ == "__main__":
    main()
