# grid_sweep.py — Joint sweep of ALPHA_Q14 × VTH shift (HW↔SW match & spike rate) (호출 폴백 포함)
import os, sys, subprocess, shutil
import numpy as np

ART = "artifacts"
T = 76
F, N = 48, 96

def have(p): return os.path.exists(p)
def run_sh(cmd):
    bash = shutil.which("bash") or "/usr/bin/bash"
    if not have(bash): sys.exit("[ERR] bash not found")
    print("[SH]", cmd)
    r = subprocess.run([bash, "-lc", cmd])
    if r.returncode != 0: sys.exit(r.returncode)

def read_hex_q14(path):
    xs=[]
    with open(path) as f:
        for ln in f:
            ln=ln.strip()
            if not ln: continue
            x=int(ln,16)
            if x & (1<<15): x -= (1<<16)
            xs.append(np.int16(x))
    return np.array(xs,dtype=np.int16)

def write_hex_q14(path, arr):
    with open(path,"w") as f:
        for v in arr.astype(np.int16):
            x = int(v)
            if x < 0: x = (1<<16) + x
            f.write(f"{x:04x}\n")

def make_vth_shift(base_hex, shift, outname=None):
    v = read_hex_q14(os.path.join(ART, base_hex))
    v2 = np.clip(v.astype(np.int32)+int(shift), -32768, 32767).astype(np.int16)
    name = outname or f"vth_shift_{shift:+d}.hex".replace("+","p")
    write_hex_q14(os.path.join(ART, name), v2)
    return name

def ensure_binary(alpha, mdir):
    if os.path.isdir(mdir):
        shutil.rmtree(mdir)
    verilator = "/mingw64/bin/verilator"
    if not have(verilator): verilator = "verilator"
    cmd = (
        f'{verilator} -sv --binary '
        f'tb_snn_mem.sv snn_core.sv lif_neuron.sv '
        f'--top-module tb_snn_mem '
        f'-Mdir {mdir} -GALPHA_Q14={alpha} '
        f'--timing'
    )
    run_sh(cmd)

def sim_hw(mdir, vth_hex, outcsv):
    run_sh(
        f'./{mdir}/Vtb_snn_mem '
        f'+EVHEX={ART}/events_ref.mem +WHEX={ART}/weights.hex +VTH={ART}/{vth_hex} '
        f'+T={T} +OUT={outcsv}'
    )

def run_sw_q14(x_csv_rel, out_csv_name, T, vth_hex_name, alpha_q14):
    # argparse 먼저 → 실패 시 positional
    x_csv_path = os.path.join(ART, x_csv_rel)
    cmd1 = [
        "python","sw_q14_from_csv.py",
        "--in", x_csv_path,
        "--out", os.path.join(ART, out_csv_name),
        "--T", str(T),
        "--vth", vth_hex_name,
        "--weights", "weights.hex",
        "--alpha", str(alpha_q14),
        "--quiet"
    ]
    r = subprocess.run(cmd1)
    if r.returncode == 0:
        return
    cmd2 = [
        "python","sw_q14_from_csv.py",
        x_csv_rel, out_csv_name, str(T), vth_hex_name, "weights.hex", str(alpha_q14)
    ]
    r2 = subprocess.run(cmd2)
    if r2.returncode != 0:
        raise SystemExit("[ERR] sw_q14_from_csv.py failed (both argparse and positional modes).")

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

def spike_rate(path_csv):
    S = np.loadtxt(path_csv, delimiter=",", dtype=int)
    return float(S.mean())

if __name__=="__main__":
    alphas = [15320, 15360, 15400, 15474, 15520, 15560, 15600]
    vth_shifts = [-4, -2, -1, 0, 1, 2, 4]

    results=[]
    base_vth = "vth.hex"

    vth_files = {}
    for s in vth_shifts:
        if s == 0:
            vth_files[s] = base_vth
        else:
            name = f"vth_shift_{s:+d}.hex".replace("+","p")
            vth_files[s] = make_vth_shift(base_vth, s, outname=name)

    for a in alphas:
        mdir = f"obj_dir_alpha_{a}"
        ensure_binary(a, mdir)
        for s in vth_shifts:
            vth_hex = vth_files[s]
            out_hw = os.path.join(ART, f"spikes_hw_a{a}_v{s:+d}.csv".replace("+","p"))
            sim_hw(mdir, vth_hex, out_hw)

            out_sw = f"spikes_sw_q14_a{a}_v{s:+d}.csv".replace("+","p")
            run_sw_q14("X_events_ref.csv", out_sw, T, vth_hex, a)

            mr = compare_csv(out_hw, os.path.join(ART, out_sw))
            sr = spike_rate(out_hw)
            results.append((a, s, mr, sr))
            print(f"[GRID] alpha={a} vth_shift={s:+d}  match={mr:.6f}  rate={sr:.6f}")

    arr = np.array(results, dtype=float)
    outp = os.path.join(ART, "grid_sweep_results.csv")
    np.savetxt(outp, arr, fmt="%.6f", delimiter=",",
               header="alpha_q14,vth_shift_LSB,match_ratio,mean_spike_rate", comments="")
    print(f"[GRID] saved {outp}")
