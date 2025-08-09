import os, json
import numpy as np

Q = 14
SCALE = 1 << Q
INT16_MIN = -32768
INT16_MAX =  32767

def to_q14(x):
    """float -> signed 16-bit two's complement in Q1.14 (클리핑 포함)."""
    v = int(round(float(x) * SCALE))
    v = max(INT16_MIN, min(INT16_MAX, v))
    if v < 0:  # two's complement
        v = (1 << 16) + v
    return v  # 0..65535 범위의 파이썬 int

def write_hex_lines(values, path):
    """values: 0..65535 범위 16비트 값을 한 줄에 4자리 HEX로 저장."""
    with open(path, "w", encoding="utf-8") as f:
        for v in values:
            f.write(f"{v:04X}\n")

def main():
    base = os.path.dirname(__file__)
    art = os.path.join(base, "artifacts")
    os.makedirs(art, exist_ok=True)

    # 1) Weights: artifacts/W_in.csv (F x N) -> artifacts/weights.hex
    w_csv = os.path.join(art, "W_in.csv")
    W = np.loadtxt(w_csv, delimiter=",", dtype=float)  # shape (F, N)
    if W.ndim != 2:
        raise RuntimeError(f"W_in.csv must be 2D, got shape {W.shape}")
    F, N = W.shape
    print(f"[GenMem] W shape: F={F}, N={N}")

    # 행 우선(row-major): feature 0의 모든 뉴런 가중치 → feature 1 … 순서
    w_q = [to_q14(w) for w in W.flatten(order="C")]
    write_hex_lines(w_q, os.path.join(art, "weights.hex"))
    print(f"[Saved] artifacts/weights.hex  (lines={len(w_q)})")

    # 2) Thresholds: artifacts/config.json → artifacts/vth.hex (N줄)
    cfg_path = os.path.join(art, "config.json")
    cfg = json.load(open(cfg_path, "r", encoding="utf-8"))
    v_th_mean = float(cfg.get("v_th_mean", 1.0))
    v_arr = cfg.get("v_th_array", None)  # 나중에 per-neuron 저장하면 이걸 사용

    if v_arr is not None:
        if len(v_arr) != N:
            raise RuntimeError(f"v_th_array length {len(v_arr)} != N {N}")
        vths = [to_q14(v) for v in v_arr]
    else:
        vths = [to_q14(v_th_mean)] * N

    write_hex_lines(vths, os.path.join(art, "vth.hex"))
    print(f"[Saved] artifacts/vth.hex       (lines={len(vths)})")

if __name__ == "__main__":
    main()
