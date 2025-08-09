import os, numpy as np
ART = os.path.join(os.path.dirname(__file__), "artifacts")

def main():
    T, F = 16, 48
    for fstar in (0, 1, 10, 23, 24, 47):
        X = np.zeros((T,F), dtype=int)
        X[2:6, fstar] = 1  # 몇 스텝만 펄스
        out = os.path.join(ART, f"X_events_single_f{fstar}.csv")
        np.savetxt(out, X, fmt="%d", delimiter=",")
        print("[Saved]", out)

if __name__ == "__main__":
    main()
