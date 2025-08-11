# alpha_sweep.py — MSYS2/Git Bash robust sweep (uses bash -lc, --binary, --timing)
import os, sys, subprocess, shutil
import numpy as np

ART = "artifacts"

def have(path): 
    return os.path.exists(path)

def run_sh(cmd_str: str):
    """Run command under bash -lc so POSIX paths/flags work on MSYS2 & Linux."""
    bash = shutil.which("bash") or "/usr/bin/bash"
    if not have(bash):
        sys.exit("[ERROR] bash not found. Install MSYS2/Git Bash or adjust PATH.")
    print("[SH]", cmd_str)
    r = subprocess.run([bash, "-lc", cmd_str])
    if r.returncode != 0:
        sys.exit(r.returncode)

def build_with_alpha(alpha: int, mdir: str):
    # --binary: build runnable binary directly; --timing: allow # delays/@events in TB
    verilator = "/mingw64/bin/verilator"
    if not have(verilator):
        verilator = "verilator"  # PATH fallback (Linux CI)
    cmd = (
        f'{verilator} -sv --binary '
        f'tb_snn_mem.sv snn_core.sv lif_neuron.sv '
        f'--top-module tb_snn_mem '
        f'-Mdir {mdir} -GALPHA_Q14={alpha} '
        f'--timing'
    )
    run_sh(cmd)   # creates {mdir}/Vtb_snn_mem

def sim_with_alpha(mdir: str, outcsv: str, T: int = 76):
    exe = os.path.join(mdir, "Vtb_snn_mem")
    ev = os.path.join(ART, "events_ref.mem")
    wh = os.path.join(ART, "weights.hex")
    vt = os.path.join(ART, "vth.hex")
    cmd = f'./{exe} +EVHEX={ev} +WHEX={wh} +VTH={vt} +T={T} +OUT={outcsv}'
    run_sh(cmd)

def spike_rate(spikes_csv: str) -> float:
    S = np.loadtxt(spikes_csv, delimiter=",", dtype=int)
    return float(S.mean())

if __name__ == "__main__":
    os.makedirs(ART, exist_ok=True)
    # 필요시 알파 값 조정
    alphas = [15360, 15474, 15600]
    results = []
    for a in alphas:
        mdir = f"obj_dir_alpha_{a}"
        build_with_alpha(a, mdir)
        out_hw = os.path.join(ART, f"spikes_hw_alpha{a}.csv")
        sim_with_alpha(mdir, out_hw, T=76)
        results.append((a, spike_rate(out_hw)))

    out_path = os.path.join(ART, "alpha_sweep_hw_rates.csv")
    np.savetxt(out_path, np.array(results, dtype=float), fmt="%.6f", delimiter=",",
               header="ALPHA_Q14,mean_spike_rate", comments="")
    print(f"[Sweep] saved {out_path}")
