# try_shift2.py
import numpy as np

hw = np.loadtxt("artifacts/spikes_hw.csv", delimiter=",", dtype=int)
sw = np.loadtxt("artifacts/spikes_sw_q14.csv", delimiter=",", dtype=int)
T, N = hw.shape

def match(a,b): return 1.0 - np.mean(a!=b)

cands = [4,66,79]

print("[Base]               ", match(hw, sw))

# HW 전체 -1 (한 싸이클 늦게 본다고 가정)
hw_p1 = np.vstack([hw[0:1], hw[:-1]])     # 앞에 첫줄 복제, 나머지 한 칸 뒤로
print("[HW -1 step ALL]     ", match(hw_p1, sw))

# HW 선택 뉴런만 -1
hw_sel_m1 = hw.copy()
for n in cands:
    hw_sel_m1[1:, n] = hw[:-1, n]
print("[HW -1 step SELECT]  ", match(hw_sel_m1, sw))

# SW 전체 +1 (SW가 한 싸이클 앞서 기록했다고 가정)
sw_m1 = np.vstack([sw[1:], sw[-1:]])
print("[SW +1 step ALL]     ", match(hw, sw_m1))

# SW 선택 뉴런만 +1
sw_sel_p1 = sw.copy()
for n in cands:
    sw_sel_p1[:-1, n] = sw[1:, n]
print("[SW +1 step SELECT]  ", match(hw, sw_sel_p1))
