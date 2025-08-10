# sw_q14_from_csv.py  (CLI 인자: <csv_in> <out_csv> [T] [vth_hex] [w_hex] [alpha_q14])
import os, sys, numpy as np
ART = os.path.join(os.path.dirname(__file__), "artifacts")

def read_hex_q14(path):
    vals=[]
    with open(path) as f:
        for ln in f:
            ln=ln.strip()
            if not ln: continue
            x=int(ln,16)
            if x & (1<<15): x -= (1<<16)
            vals.append(np.int16(x))
    return np.array(vals, dtype=np.int16)

def q14_sat16(x):
    if x>32767: return np.int16(32767)
    if x<-32768: return np.int16(-32768)
    return np.int16(x)

def run(csv_in, out_csv, F=48, N=96, T=None, Q=14, alpha_q14=15474, refrac_steps=2,
        w_hex="weights.hex", vth_hex="vth.hex"):
    X = np.loadtxt(os.path.join(ART,csv_in), delimiter=",", dtype=int)
    if X.ndim!=2 or X.shape[1]!=F: raise SystemExit(f"stim shape {X.shape} != (T,{F})")
    if T is None: T=X.shape[0]
    Wf = read_hex_q14(os.path.join(ART,w_hex))
    Vth= read_hex_q14(os.path.join(ART,vth_hex))
    if Wf.size!=F*N or Vth.size!=N: raise SystemExit("HEX size mismatch")
    W = Wf.reshape(F,N)
    V = np.zeros((N,),dtype=np.int16); refr=np.zeros((N,),dtype=np.int32)
    spikes = np.zeros((T,N),dtype=int)

    for t in range(T):
        leak32 = np.int32(alpha_q14)*np.int32(V)  # Q2.28
        idx = np.nonzero(X[t])[0]
        acc32 = np.zeros((N,),dtype=np.int32)
        if idx.size>0: acc32 = np.sum(np.int32(W[idx,:]), axis=0)  # widen
        bias = np.where(leak32>=0, (1<<(Q-1)), -(1<<(Q-1))).astype(np.int32)
        leak_q14 = ((leak32 + bias) >> Q).astype(np.int32)
        s32 = leak_q14 + acc32
        Vn = np.array([q14_sat16(int(s)) for s in s32], dtype=np.int16)

        active = (refr<=0)
        fire = np.logical_and(active, np.int32(Vn)>=np.int32(Vth))
        spikes[t, fire]=1

        V = Vn; V[fire]=np.int16(0)
        refr[fire]=refrac_steps; refr[~fire]=np.maximum(0,refr[~fire]-1)

    outp = os.path.join(ART,out_csv)
    np.savetxt(outp, spikes, fmt="%d", delimiter=",")
    print(f"[SW-Q14] saved {outp}")

if __name__=="__main__":
    if len(sys.argv)<3:
        print("Usage: python sw_q14_from_csv.py <X_events_*.csv> <out_spikes.csv> [T] [vth_hex] [w_hex] [alpha_q14]")
        sys.exit(1)
    csv_in = sys.argv[1]; out_csv = sys.argv[2]
    T = int(sys.argv[3]) if len(sys.argv)>=4 and sys.argv[3] else None
    vth_hex = sys.argv[4] if len(sys.argv)>=5 and sys.argv[4] else "vth.hex"
    w_hex   = sys.argv[5] if len(sys.argv)>=6 and sys.argv[5] else "weights.hex"
    alpha_q14 = int(sys.argv[6]) if len(sys.argv)>=7 and sys.argv[6] else 15474
    run(csv_in, out_csv, T=T, vth_hex=vth_hex, w_hex=w_hex, alpha_q14=alpha_q14)
