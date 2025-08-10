# try_shift.py
import numpy as np

hw = np.loadtxt("artifacts/spikes_hw.csv", delimiter=",", dtype=int)
sw = np.loadtxt("artifacts/spikes_sw_q14.csv", delimiter=",", dtype=int)

def match(a,b): return 1.0 - np.mean(a!=b)

print("[Base] match =", match(hw, sw))

# 후보 뉴런(짝반전 난 자리)
cands = [4,66,79]

# (A) 전체 1스텝 앞으로 당겨보기(HW 기준)
hw_m1 = np.vstack([hw[1:], hw[-1:]])     # 마지막은 복제
print("[Shift A: HW→+1 step] match =", match(hw_m1, sw))

# (B) 해당 뉴런만 1스텝 시프트
hw_sel = hw.copy()
for n in cands:
    hw_sel[1:, n] = hw[:-1, n]
print("[Shift B: HW (selected neurons)→+1 step] match =", match(hw_sel, sw))
