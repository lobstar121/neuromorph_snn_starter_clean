import os, json
import numpy as np
ART = os.path.join(os.path.dirname(__file__), "artifacts")

def main():
    X = np.loadtxt(os.path.join(ART, "X_events_ref.csv"), delimiter=",", dtype=float)  # (T,F)
    W = np.loadtxt(os.path.join(ART, "W_in.csv"), delimiter=",", dtype=float)          # (F,N)
    cfg = json.load(open(os.path.join(ART, "config.json"), "r", encoding="utf-8"))
    p = cfg["params"]; v_th = float(cfg.get("v_th_mean", p.get("v_th", 1.0)))
    dt, tau_m = p.get("dt", 0.002), p.get("tau_m", 0.035)
    alpha = np.exp(-dt / tau_m)

    T,F = X.shape; Fw,N = W.shape
    assert F==Fw
    V = np.zeros((N,), dtype=np.float64)
    refrac = np.zeros((N,), dtype=np.int32)
    refrac_steps = int(p.get("refrac", 0.004)/dt)
    lsb = 1.0/(1<<14)

    records = []
    for t in range(T):
        V = alpha*V + X[t].astype(np.float64) @ W  # dot -> shape (N,)
        active = refrac<=0
        fired = (V >= v_th) & active
        margin = V - v_th
        # t=25 주변/전체에서 마진이 LSB 몇 배인지 기록
        for n in range(N):
            m = margin[n]
            if abs(m) <= 8*lsb:  # 경계 근처만 축출
                records.append((t,n,m))
        # reset/refrac
        V[fired] = p.get("v_reset", 0.0)
        refrac[fired] = refrac_steps
        refrac[~fired] = np.maximum(0,refrac[~fired]-1)

    # 정렬: |margin| 작은 순
    records.sort(key=lambda x: abs(x[2]))
    print(f"[margins] near-boundary count={len(records)}  (show top 20)")
    for t,n,m in records[:20]:
        print(f"  t={t:3d} n={n:3d} margin={m:+.8f}  (~{abs(m)/lsb:.2f} LSB)")

if __name__ == "__main__":
    main()
