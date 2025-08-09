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

# 단일-특성 스모크(원하면 추가)
SINGLE_CSV=$(ART)/X_events_single_f0.csv $(ART)/X_events_single_f1.csv $(ART)/X_events_single_f23.csv $(ART)/X_events_single_f24.csv
SINGLE_MEM=$(SINGLE_CSV:.csv=.mem)

# ====== Build RTL sim ======
OBJDIR=obj_dir
SIM=$(OBJDIR)/Vtb_snn_mem

.PHONY: all test golden hw swq14 compare smoke clean veryclean

all: test

$(SIM): tb_snn_mem.sv snn_core.sv lif_neuron.sv
	verilator -sv --cc tb_snn_mem.sv snn_core.sv lif_neuron.sv --top-module tb_snn_mem
	$(MAKE) -C $(OBJDIR) -f Vtb_snn_mem.mk PYTHON3=python

# ====== 1) 회귀 테스트 고정 ======
# 이벤트 CSV -> MEM
$(EV_MEM): $(EV_REF)
	python csv2mem.py $(EV_REF) $(F) $(EV_MEM)

# HW 실행
hw: $(SIM) $(EV_MEM) $(WHEX) $(VTH)
	$(SIM) +EVHEX=$(EV_MEM) +WHEX=$(WHEX) +VTH=$(VTH) +T=$(T) +OUT=$(HW_OUT)

# SW(Q1.14) 생성
swq14:
	python fixedpoint_replay.py

# 비교 (HW vs SW_Q14)
compare:
	python compare_spikes.py $(HW_OUT) $(SW_Q14)

# 단일 타깃: 테스트(1회) = csv2mem -> hw -> swq14 -> compare
test: $(EV_MEM) hw swq14 compare

# ====== 2) 골든 관리 ======
# 현재 SW_Q14를 골든으로 고정
golden: swq14
	cp -f $(SW_Q14) $(GOLD)
	@echo "[GOLD] updated: $(GOLD)"

# 골든과 비교하고 싶을 때 (HW vs GOLD)
compare_golden: hw
	python compare_spikes.py $(HW_OUT) $(GOLD)

# ====== 3) 스모크 (옵션) ======
# 단일-특성 CSV -> MEM 변환
$(ART)/events_%.mem: $(ART)/X_events_%.csv
	python csv2mem.py $< $(F) $@

smoke: $(SIM) $(WHEX) $(VTH) $(SINGLE_MEM)
	@for M in $(SINGLE_MEM); do \
	  BASE=$${M%.mem}; \
	  OUT=$(ART)/spikes_hw_$${BASE##*/}.csv; \
	  echo "[SMOKE] $$M -> $$OUT"; \
	  $(SIM) +EVHEX=$$M +WHEX=$(WHEX) +VTH=$(VTH) +T=16 +OUT=$$OUT; \
	done
	@echo "[SMOKE] done."

clean:
	@rm -f $(ART)/spikes_hw*.csv $(ART)/events_*.mem $(ART)/diff_mask.csv

veryclean: clean
	@rm -rf $(OBJDIR)
