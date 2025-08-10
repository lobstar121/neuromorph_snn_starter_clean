\
.RECIPEPREFIX := >
# ====== Config ======
ART=artifacts
F=48
N=96
T=76
EV_REF=$(ART)/X_events_ref.csv
EV_MEM=$(ART)/events_ref.mem
WHEX=$(ART)/weights.hex
VTH=$(ART)/vth.hex
HW_OUT=$(ART)/spikes_hw.csv
SW_Q14=$(ART)/spikes_sw_q14.csv
GOLD=$(ART)/golden_spikes.csv

# Toolchain / build opts
VERILATOR ?= verilator
TIMING    ?= --timing
ALPHA_Q14 ?= 15520

TOP       := tb_snn_mem
SRC       := tb_snn_mem.sv snn_core.sv lif_neuron.sv

# 단일-특성 스모크 입력
SINGLE_CSV=$(ART)/X_events_single_f0.csv $(ART)/X_events_single_f1.csv $(ART)/X_events_single_f23.csv $(ART)/X_events_single_f24.csv
SINGLE_MEM=$(SINGLE_CSV:.csv=.mem)

# ====== Build RTL sim ======
OBJDIR=obj_dir
SIM=$(OBJDIR)/V$(TOP)

.PHONY: all test golden hw swq14 compare smoke clean veryclean smoke_compare report alpha_sweep vth_sweep grid_sweep release compare_golden

all: test

# Verilator 5.x: --binary + --timing + parameter injection
$(SIM): $(SRC)
> $(VERILATOR) -sv --binary $(SRC) --top-module $(TOP) -Mdir $(OBJDIR) -GALPHA_Q14=$(ALPHA_Q14) $(TIMING)

# ====== 1) 회귀 테스트 고정 ======
$(EV_MEM): $(EV_REF)
> python csv2mem.py $(EV_REF) $(F) $(EV_MEM)

hw: $(SIM) $(EV_MEM) $(WHEX) $(VTH)
> $(SIM) +EVHEX=$(EV_MEM) +WHEX=$(WHEX) +VTH=$(VTH) +T=$(T) +OUT=$(HW_OUT)

swq14:
> python fixedpoint_replay.py

compare:
> python compare_spikes.py $(HW_OUT) $(SW_Q14)

test: $(EV_MEM) hw swq14 compare

# ====== 2) 골든 관리 ======
golden: swq14
> cp -f $(SW_Q14) $(GOLD)
> @echo "[GOLD] updated: $(GOLD)"

compare_golden: hw
> python compare_spikes.py $(HW_OUT) $(GOLD)

# ====== 3) 스모크 ======
$(ART)/events_%.mem: $(ART)/X_events_%.csv
> python csv2mem.py $< $(F) $@

smoke: $(SIM) $(WHEX) $(VTH) $(SINGLE_MEM)
> @for M in $(SINGLE_MEM); do \
>   BASE=$${M%.mem}; \
>   OUT=$(ART)/spikes_hw_$${BASE##*/}.csv; \
>   echo "[SMOKE] $$M -> $$OUT"; \
>   $(SIM) +EVHEX=$$M +WHEX=$(WHEX) +VTH=$(VTH) +T=16 +OUT=$$OUT; \
> done
> @echo "[SMOKE] done."

smoke_compare: $(SIM) $(WHEX) $(VTH) $(SINGLE_MEM)
> @for CSV in $(SINGLE_CSV); do \
>   BASE=$${CSV##*/}; NAME=$${BASE%.csv}; \
>   echo "[SMOKE-COMPARE] $$NAME"; \
>   python sw_q14_from_csv.py $$BASE spikes_sw_q14_$${NAME}.csv 16; \
>   $(SIM) +EVHEX=$(ART)/$${NAME}.mem +WHEX=$(WHEX) +VTH=$(VTH) +T=16 +OUT=$(ART)/spikes_hw_$${NAME}.csv; \
>   python compare_spikes.py $(ART)/spikes_hw_$${NAME}.csv $(ART)/spikes_sw_q14_$${NAME}.csv; \
> done
> @echo "[SMOKE-COMPARE] done."

report:
> python spike_report.py artifacts/spikes_hw.csv hw
> python spike_report.py artifacts/spikes_sw_q14.csv swq14

alpha_sweep:
> PATH=/mingw64/bin:$(PATH) python alpha_sweep.py

vth_sweep:
> PATH=/mingw64/bin:$(PATH) python vth_sweep.py

grid_sweep:
> PATH=/mingw64/bin:$(PATH) python grid_sweep.py

clean:
> @rm -f $(ART)/spikes_hw*.csv $(ART)/events_*.mem $(ART)/diff_mask.csv

veryclean: clean
> @rm -rf $(OBJDIR)

# ====== 4) Release package ======
release: $(SIM) $(EV_MEM) $(WHEX) $(VTH) $(GOLD)
> @echo "[REL] assembling release/"
> @rm -rf release
> @mkdir -p release/artifacts
> @cp -f $(SIM) release/
> @cp -f tb_snn_mem.sv snn_core.sv lif_neuron.sv Makefile release/
> @cp -f fixedpoint_replay.py compare_spikes.py csv2mem.py sw_q14_from_csv.py release/
> @cp -f $(ART)/weights.hex release/artifacts/
> @cp -f $(ART)/vth.hex release/artifacts/
> @cp -f $(ART)/events_ref.mem release/artifacts/
> @cp -f $(ART)/golden_spikes.csv release/artifacts/
> @python release_readme.py
> @echo "[REL] done."
