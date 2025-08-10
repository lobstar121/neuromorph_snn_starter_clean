# Neuromorph SNN — Release

## What’s inside
- `Vtb_snn_mem` : Verilated 실행 바이너리
- `tb_snn_mem.sv, snn_core.sv, lif_neuron.sv` : RTL
- `artifacts/weights.hex, vth.hex, events_ref.mem, golden_spikes.csv` : 고정 아티팩트
- `fixedpoint_replay.py, compare_spikes.py, csv2mem.py, sw_q14_from_csv.py, Makefile` : 재현 스크립트

## Quick check (HW→GOLD)
./Vtb_snn_mem \
  +EVHEX=artifacts/events_ref.mem \
  +WHEX=artifacts/weights.hex \
  +VTH=artifacts/vth.hex \
  +T=76 \
  +OUT=artifacts/spikes_hw.csv

python compare_spikes.py artifacts/spikes_hw.csv artifacts/golden_spikes.csv

## HW vs SW(Q14) 재현
python fixedpoint_replay.py
python compare_spikes.py artifacts/spikes_hw.csv artifacts/spikes_sw_q14.csv

## Notes
- Verilator 5.x 기준으로 빌드됨 (--timing 필요)
- ALPHA_Q14 = 15520 (plateau center)
- GOLD = artifacts/golden_spikes.csv
