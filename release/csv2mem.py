# csv2mem.py
import os, sys
import numpy as np

def csv_to_hex_lines(csv_path, F):
    X = np.loadtxt(csv_path, delimiter=",", dtype=int)  # shape (T, F)
    if X.ndim != 2 or X.shape[1] != F:
        raise SystemExit(f"CSV shape {X.shape} != (T,{F})")
    T = X.shape[0]
    lines = []
    for t in range(T):
        # LSB<-feature0 ... MSB<-feature(F-1) 로 맵핑 (TB와 동일하게)
        val = 0
        for f in range(F):
            if X[t, f] != 0:
                val |= (1 << f)
        lines.append(f"{val:0{(F+3)//4}X}")
    return lines, T

def main():
    if len(sys.argv) < 4:
        print("Usage: python csv2mem.py <X_events.csv> <F> <out_hex.mem>")
        sys.exit(1)
    csv_path = sys.argv[1]
    F = int(sys.argv[2])
    out_path = sys.argv[3]
    lines, T = csv_to_hex_lines(csv_path, F)
    os.makedirs(os.path.dirname(out_path) or ".", exist_ok=True)
    with open(out_path, "w") as f:
        for ln in lines:
            f.write(ln + "\n")
    print(f"[csv2mem] F={F}, T={T}, wrote {out_path}")
    # 길이는 .len 파일로도 저장
    with open(out_path + ".len", "w") as f:
        f.write(str(T) + "\n")

if __name__ == "__main__":
    main()
