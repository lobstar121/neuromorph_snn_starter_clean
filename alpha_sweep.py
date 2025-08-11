# alpha_sweep.py — MSYS2/Git Bash robust sweep (uses bash -lc, --binary, --timing)
import os, sys, subprocess, shutil
import numpy as np

ART = "artifacts"

def have(path):
    return os.path.exists(path)

def run_sh(cmd_str: str):
    """Run command under bash -lc so POSIX paths/flags work on MSYS2/Ubuntu."""
    bash = shutil.which("bash") or "/usr/bin/bash"
    if not have(bash):
        sys.exit("[ERROR] bash not found. Install MSYS2/Git Bash or adjust PATH.")
    print("[SH]", cmd_str)
    r = subprocess.run([bash, "-lc", cmd_str])
    if r.returncode != 0:
        sys.exit(r.returncode)

def build_with_alpha(alpha: int, mdir: str):
    # A) 빌드 전 대상 Mdir 강제 삭제 → 오래된 Make 조각/오브젝트로 인한 충돌 방지
    if os.path.isdir(mdir):
        shutil.rmtree(mdir)
    # --binary: 실행파일까지 한 번에 생성, --timing: TB의 #delay/@event 허용
    verilator = "/mingw64/bin/verilator"
    if not have(verilator):
        verilator = "verilator"  # PATH에 있으면 이것도 OK
    cmd = (
        f'{verilator} -sv --binary '
        f'tb_snn_mem.sv snn_core.sv lif_neuron.sv '
        f'--top-module tb_snn_mem '
        f'-Mdir {mdir} -GALPHA_Q14={alpha} '
        f'--timing'
    )
    run_sh(cmd)   # 실행파일: {mdir}/Vtb_snn_mem 생성됨

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
    # 필요시 알파 값 수정
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
