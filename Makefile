.ONESHELL:
.RECIPEPREFIX := >
# ====== Config ======
ART=artifacts
F=48
N=96
T=76
EV_REF=$(ART)/X_events_ref.csv
EV_MEM=$(ART)/events_ref.mem
WHEX=$(ART)/weights.hex

# --- 선택형 파라미터 자동 반영 ---
ALPHA_FILE := $(ART)/alpha_selected.txt
VTH_SEL    := $(ART)/vth_selected.hex
# 기본값
ALPHA_Q14 ?= 15520
VTH       := $(ART)/vth.hex
# 파일이 있으면 우선 사용
ifneq ("$(wildcard $(ALPHA_FILE))","")
ALPHA_Q14 := $(shell sed -n '1p' $(ALPHA_FILE))
endif
ifneq ("$(wildcard $(VTH_SEL))","")
VTH := $(VTH_SEL)
endif

HW_OUT=$(ART)/spikes_hw.csv
SW_Q14=$(ART)/spikes_sw_q14.csv
GOLD=$(ART)/golden_spikes.csv

# Toolchain / build opts
VERILATOR ?= verilator
TIMING    ?= --timing
export ALPHA_Q14

TOP       := tb_snn_mem
SRC       := tb_snn_mem.sv snn_core.sv lif_neuron.sv stdp_q14.sv

# 단일-특성 스모크 입력
SINGLE_CSV=$(ART)/X_events_single_f0.csv $(ART)/X_events_single_f1.csv $(ART)/X_events_single_f23.csv $(ART)/X_events_single_f24.csv
SINGLE_MEM=$(SINGLE_CSV:.csv=.mem)

.PHONY: all test golden hw swq14 compare smoke clean veryclean smoke_compare report \
        alpha_sweep vth_sweep grid_sweep release compare_golden analyze finalize \
        selfcheck ci \
        learn_clean learn_smoke learn_grid learn_select learn_tune

swq14:
> set -e
> python fixedpoint_replay.py --alpha $(ALPHA_Q14)

# ====== Build RTL sim ======
OBJDIR=obj_dir
SIM=$(OBJDIR)/V$(TOP)

all: test

$(SIM): $(SRC)
> set -e
> $(VERILATOR) -sv --binary $(SRC) --top-module $(TOP) -Mdir $(OBJDIR) -GALPHA_Q14=$(ALPHA_Q14) $(TIMING)

# ====== 1) 회귀 테스트 ======
$(EV_MEM): $(EV_REF)
> set -e
> python csv2mem.py $(EV_REF) $(F) $(EV_MEM)

hw: $(SIM) $(EV_MEM) $(WHEX) $(VTH)
> set -e
> $(SIM) +EVHEX=$(EV_MEM) +WHEX=$(WHEX) +VTH=$(VTH) +T=$(T) +OUT=$(HW_OUT)

compare:
> set -e
> python compare_spikes.py $(HW_OUT) $(SW_Q14)

test: $(EV_MEM) hw swq14 compare

# ====== 2) 골든 ======
golden: swq14
> set -e
> cp -f $(SW_Q14) $(GOLD)
> echo "[GOLD] updated: $(GOLD)"

compare_golden: hw
> set -e
> python compare_spikes.py $(HW_OUT) $(GOLD)

# ====== 3) 스모크 ======
$(ART)/events_%.mem: $(ART)/X_events_%.csv
> set -e
> python csv2mem.py $< $(F) $@

smoke: $(SIM) $(WHEX) $(VTH) $(SINGLE_MEM)
> set -e
> for M in $(SINGLE_MEM); do
>   BASE=$${M%.mem}
>   OUT=$(ART)/spikes_hw_$${BASE##*/}.csv
>   echo "[SMOKE] $$M -> $$OUT"
>   $(SIM) +EVHEX=$$M +WHEX=$(WHEX) +VTH=$(VTH) +T=16 +OUT=$$OUT
> done
> echo "[SMOKE] done."

smoke_compare: $(SIM) $(WHEX) $(VTH) $(SINGLE_MEM)
> set -e
> for CSV in $(SINGLE_CSV); do
>   BASE=$${CSV##*/}
>   NAME=$${BASE%.csv}
>   echo "[SMOKE-COMPARE] $$NAME"
>   python sw_q14_from_csv.py $$BASE spikes_sw_q14_$${NAME}.csv 16
>   $(SIM) +EVHEX=$(ART)/$${NAME}.mem +WHEX=$(WHEX) +VTH=$(VTH) +T=16 +OUT=$(ART)/spikes_hw_$${NAME}.csv
>   python compare_spikes.py $(ART)/spikes_hw_$${NAME}.csv $(ART)/spikes_sw_q14_$${NAME}.csv
> done
> echo "[SMOKE-COMPARE] done."

report:
> set -e
> python spike_report.py artifacts/spikes_hw.csv hw
> python spike_report.py artifacts/spikes_sw_q14.csv swq14

# ====== 4) Tuning Sweeps ======
alpha_sweep:
> set -e
> PATH=/mingw64/bin:$(PATH) python alpha_sweep.py

vth_sweep:
> set -e
> PATH=/mingw64/bin:$(PATH) python vth_sweep.py

grid_sweep:
> set -e
> PATH=/mingw64/bin:$(PATH) python grid_sweep.py

analyze:
> set -e
> python analyze_sweeps.py

finalize: alpha_sweep vth_sweep grid_sweep analyze
> echo "[FINALIZE] alpha/VTH selection completed."

clean:
> rm -f $(ART)/spikes_hw*.csv $(ART)/events_*.mem $(ART)/diff_mask.csv

veryclean: clean
> rm -rf $(OBJDIR)

# ====== 5) Release ======
release: $(SIM) $(EV_MEM) $(WHEX) $(VTH) $(GOLD)
> set -e
> echo "[REL] assembling release/"
> rm -rf release
> mkdir -p release/artifacts
> cp -f $(SIM) release/
> cp -f tb_snn_mem.sv snn_core.sv lif_neuron.sv stdp_q14.sv Makefile release/
> cp -f fixedpoint_replay.py compare_spikes.py csv2mem.py sw_q14_from_csv.py analyze_sweeps.py release/
> cp -f $(ART)/weights.hex release/artifacts/
> cp -f $(ART)/vth.hex release/artifacts/
> cp -f $(ART)/events_ref.mem release/artifacts/
> cp -f $(ART)/golden_spikes.csv release/artifacts/
> python release_readme.py
> echo "[REL] done."

# ====== 6) CI 기본 검사 ======
selfcheck: test
> set -e
> python assert_match.py --expect 1.0 artifacts/spikes_hw.csv artifacts/spikes_sw_q14.csv

ci: veryclean selfcheck

# ====== 7) STDP SW 학습 스모크 ======
learn_clean:
> rm -f artifacts/weights_learned.hex artifacts/spikes_sw_learn.csv

learn_smoke: $(ART)/X_events_ref.csv $(ART)/weights.hex $(ART)/vth.hex
> set -e
> python learn_stdp_sw.py \
>   --in artifacts/X_events_ref.csv \
>   --weights-in artifacts/weights.hex \
>   --weights-out artifacts/weights_learned.hex \
>   --vth artifacts/vth.hex \
>   --F $(F) --N $(N) --T 64 --alpha $(ALPHA_Q14) --refrac 2 --thresh-mode ge \
>   --eta 8 --eta-shift 12 --lambda-x 15565 --lambda-y 15565 --b-pre 1024 --b-post 1024 \
>   --save-spikes artifacts/spikes_sw_learn.csv
> echo "[LEARN] outputs: artifacts/weights_learned.hex, artifacts/spikes_sw_learn.csv"

# ====== 8) STDP 파라미터 튜닝 (grid -> select) ======
ETAS        ?= 4 8
ETA_SHIFTS  ?= 12 13
LAMBDAS     ?= 15500 15565
B_PRE       ?= 1024
B_POST      ?= 1024
LEARN_T     ?= 64

learn_grid: $(ART)/X_events_ref.csv $(ART)/weights.hex $(VTH)
> set -e
> echo "[LEARN-GRID] sweep start"
> rm -f $(ART)/learn_grid.csv
> echo "eta,eta_shift,lambda_x,lambda_y,b_pre,b_post,spike_sum,weight_l1,out_hex" > $(ART)/learn_grid.csv
> for e in $(ETAS); do
>   for s in $(ETA_SHIFTS); do
>     for lx in $(LAMBDAS); do
>       for ly in $(LAMBDAS); do
>         OUTHEX=$(ART)/weights_learn_e$${e}_s$${s}_lx$${lx}_ly$${ly}.hex
>         OUTSPI=$(ART)/spikes_sw_learn_e$${e}_s$${s}_lx$${lx}_ly$${ly}.csv
>         echo "[LEARN] eta=$${e} s=$${s} lx=$${lx} ly=$${ly} -> $$OUTHEX"
>         python learn_stdp_sw.py \
>           --in $(ART)/X_events_ref.csv \
>           --weights-in $(ART)/weights.hex \
>           --weights-out $$OUTHEX \
>           --vth $(VTH) \
>           --F $(F) --N $(N) --T $(LEARN_T) --alpha $(ALPHA_Q14) --refrac 2 --thresh-mode ge \
>           --eta $$e --eta-shift $$s --lambda-x $$lx --lambda-y $$ly --b-pre $(B_PRE) --b-post $(B_POST) \
>           --save-spikes $$OUTSPI
>         # 메트릭 행 추가 (heredoc)
>         python - "$$OUTHEX" "$(ART)/weights.hex" "$$OUTSPI" "$(ART)/learn_grid.csv" "$$e" "$$s" "$$lx" "$$ly" "$(B_PRE)" "$(B_POST)" << 'PY'
import sys, numpy as np
outhex, basehex, outspi, csv, e, s, lx, ly, bp, bq = sys.argv[1:]
def read_hex(path):
    xs=[]
    with open(path) as f:
        for ln in f:
            ln=ln.strip()
            if not ln: continue
            x=int(ln,16)
            if x & (1<<15): x-=(1<<16)
            xs.append(x)
    return np.array(xs, dtype=np.int16)
Wb = read_hex(basehex)
Wn = read_hex(outhex)
l1 = int(np.abs(Wn.astype(np.int32)-Wb.astype(np.int32)).sum())
spikes = np.loadtxt(outspi, delimiter=",", dtype=int)
spike_sum = int(spikes.sum()) if spikes.size else 0
with open(csv,"a") as f:
    f.write(f"{e},{s},{lx},{ly},{bp},{bq},{spike_sum},{l1},{outhex}\n")
PY
>       done
>     done
>   done
> done
> echo "[LEARN-GRID] wrote $(ART)/learn_grid.csv"

learn_select: $(ART)/learn_grid.csv
> set -e
> python - "$(ART)/learn_grid.csv" "$(ART)/weights_learned_best.hex" "$(ART)/learn_selected.json" << 'PY'
import sys, json, numpy as np
csv, dst, meta = sys.argv[1:]
rows=[]
with open(csv) as f:
    header=f.readline()
    for ln in f:
        if not ln.strip(): continue
        eta,esh,lx,ly,bp,bq,ss,l1,out = ln.strip().split(",")
        rows.append(dict(eta=int(eta),eta_shift=int(esh),
                         lambda_x=int(lx),lambda_y=int(ly),
                         b_pre=int(bp),b_post=int(bq),
                         spike_sum=int(ss),weight_l1=int(l1),
                         out_hex=out))
if not rows:
    print("[select] grid empty"); sys.exit(2)
cands=[r for r in rows if r["spike_sum"]>0]
cands = cands or rows
best = sorted(cands, key=lambda r:(-r["spike_sum"], r["weight_l1"]))[0]
with open(best["out_hex"]) as s, open(dst,"w") as d:
    d.write(s.read())
with open(meta,"w") as jf:
    json.dump(best, jf, indent=2)
print("[select] best:", best)
PY
> echo "[LEARN-SELECT] -> $(ART)/weights_learned_best.hex , $(ART)/learn_selected.json"

learn_tune: learn_grid learn_select
> echo "[LEARN-TUNE] done."
