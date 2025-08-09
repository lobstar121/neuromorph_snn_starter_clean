# vth_sweep.py — VTH(hex) ±LSB 스윕: HW↔SW(Q14) 매칭 & 발화율 수집
import os, sys, subprocess, shutil
import numpy as np

ART = "artifacts"
ALPHA = 15474  # 1단계에서 고른 값 고정
T = 76
F, N = 48, 96

def have(p): return os.path.exists(p)

def run_sh(cmd):
    bash = shutil.which("bash") or "/usr/bin/bash"
    if not have(bash): sys.exit("[ERROR] bash not found")
    print("[SH]", cmd)
    r = subprocess.run([bash,"-lc",cmd])
    if r.returncode!=0: sys.exit(r.returncode)

def read_hex_q14(path):
    xs=[]
    with open(path) as f:
        for ln in f:
            ln=ln.strip()
            if not ln: continue
            x=int(ln,16)
            if x & (1<<15): x -= (1<<16)
            xs.append(np.int16(x))
    arr=np.array(xs,dtype=np.int16)
    if arr.size!=N: sys.exit(f"[ERR] vth size {arr.size} != {N}")
    return arr

def write_hex_q14(path, arr):
    with open(path,"w") as f:
        for v in arr:
            v16 = np.int16(v).astype(np.int32)
            if v16<0: v16 = (1<<16)+v16
            f.write(f"{v16:04x}\n")

def make_shifted(base_hex, shift):
    v = read_hex_q14(os.path.join(ART, base_hex))
    v2 = np.clip(v.astype(np.int32)+int(shift), -32768, 32767).astype(np.int16)
    name = f"vth_shift_{shift:+d}.hex".replace("+","p")
    outp = os.path.join(ART, name)
    write_hex_q14(outp, v2)
    return name

def spike_rate(path_csv):
    S = np.loadtxt(path_csv, delimiter=",", dtype=int)
    return float(S.mean())

def compare_csv(a,b):
    r = subprocess.run(["python","compare_spikes.py", a, b],
                       capture_output=True, text=True)
    print(r.stdout)
    mr=None
    for line in r.stdout.splitlines():
        if "match ratio:" in line:
            mr = float(line.split("match ratio:")[1].split("(")[0].strip())
            break
    return mr if mr is not None else 0.0

def ensure_binary(alpha, mdir):
    verilator = "/mingw64/bin/verilator"
    if not have(verilator): verilator="verilator"
    cmd = f'{verilator} -sv --binary tb_snn_mem.sv snn_core.sv lif_neuron.sv ' \
          f'--top-module tb_snn_mem -Mdir {mdir} -GALPHA_Q14={alpha} --timing'
    run_sh(cmd)

def run_one(vth_hex_name):
    mdir = "obj_dir_alpha_15474"  # 이미 존재하면 재빌드 스킵
    ensure_binary(ALPHA, mdir)
    out_hw = os.path.join(ART, f"spikes_hw_{vth_hex_name.replace('.hex','')}.csv")
    run_sh(f'./{mdir}/Vtb_snn_mem +EVHEX={ART}/events_ref.mem +WHEX={ART}/weights.hex '
           f'+VTH={ART}/{vth_hex_name} +T={T} +OUT={out_hw}')

    out_sw = os.path.join(ART, f"spikes_sw_q14_{vth_hex_name.replace('.hex','')}.csv")
    # SW도 동일 vth로 재생
    subprocess.check_call(["python","sw_q14_from_csv.py","X_events_ref.csv",
                           os.path.basename(out_sw), str(T), vth_hex_name, "weights.hex", str(ALPHA)])

    mr = compare_csv(out_hw, out_sw)
    sr = spike_rate(out_hw)
    return mr, sr, out_hw, out_sw

if __name__=="__main__":
    # 쉬프트 후보(LSB): 필요시 조정
    shifts = [-4, -2, -1, 0, +1, +2, +4]
    results=[]
    names=[]
    for s in shifts:
        name = "vth.hex" if s==0 else make_shifted("vth.hex", s)
        names.append(name)
        mr, sr, _, _ = run_one(name)
        results.append((s, mr, sr))
    res = np.array(results, dtype=float)
    out = os.path.join(ART, "vth_sweep_results.csv")
    np.savetxt(out, res, fmt="%.6f", delimiter=",", header="shift_LSB,match_ratio,mean_spike_rate", comments="")
    print(f"[VTH-SWEEP] saved {out}")
