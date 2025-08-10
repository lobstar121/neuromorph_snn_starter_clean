# assert_match.py
import sys, numpy as np

def main():
    if len(sys.argv) < 4:
        print("Usage: python assert_match.py --expect <ratio> <hw_csv> <ref_csv>")
        sys.exit(2)
    expect = float(sys.argv[2])
    hw = np.loadtxt(sys.argv[3], delimiter=",", dtype=int)
    ref = np.loadtxt(sys.argv[4], delimiter=",", dtype=int)
    if hw.shape != ref.shape:
        print(f"[FAIL] shape mismatch: {hw.shape} vs {ref.shape}")
        sys.exit(3)
    match = 1.0 - np.mean(hw != ref)
    print(f"[ASSERT] expected={expect:.6f}  actual={match:.6f}")
    if match + 1e-12 < expect:
        print("[FAIL] match ratio below expected threshold")
        sys.exit(1)
    print("[PASS]")
    return 0

if __name__ == "__main__":
    sys.exit(main())
