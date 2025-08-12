# compare_weights.py
import sys

def read_hex16(path):
    xs=[]
    with open(path) as f:
        for ln in f:
            s=ln.strip()
            if not s: continue
            v=int(s,16)
            if v & (1<<15): v -= (1<<16)
            xs.append(v)
    return xs

if __name__ == "__main__":
    if len(sys.argv)!=3:
        print("usage: python compare_weights.py sw.hex rtl.hex"); sys.exit(2)
    a=read_hex16(sys.argv[1])
    b=read_hex16(sys.argv[2])
    if len(a)!=len(b):
        print(f"[PARITY] size mismatch: {len(a)} vs {len(b)}"); sys.exit(3)
    diff=sum(1 for i in range(len(a)) if a[i]!=b[i])
    print(f"[PARITY] weights equal: {diff==0} (diff_count={diff})")
    sys.exit(0 if diff==0 else 1)
