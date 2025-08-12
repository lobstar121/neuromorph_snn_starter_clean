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

# compare_weights.py
import sys

def read_hex(path):
    xs=[]
    with open(path) as f:
        for ln in f:
            s=ln.strip()
            if not s: continue
            v=int(s,16)
            if v & (1<<15): v -= (1<<16)
            xs.append(v)
    return xs

def main():
    if len(sys.argv)<3:
        print("usage: compare_weights.py A.hex B.hex")
        sys.exit(2)
    A=read_hex(sys.argv[1])
    B=read_hex(sys.argv[2])
    if len(A)!=len(B):
        print(f"len mismatch: {len(A)} vs {len(B)}")
        sys.exit(2)
    mism=0
    for i,(a,b) in enumerate(zip(A,B)):
        if a!=b: mism+=1
    print(f"[PARITY] weight mismatch count = {mism} / {len(A)}")
    sys.exit(0 if mism==0 else 1)

if __name__=="__main__":
    main()
