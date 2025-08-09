# Neuromorph SNN Starter (Audio Keyword Spotting, A-Plan)

This is a minimal, **DIY-friendly** starter kit for a spiking neural network (SNN) prototype using **discrete-time LIF neurons** in NumPy. It targets **keyword spotting ("yes" vs "no")** with:
- Event-based preprocessing from audio (librosa)
- SNN forward with fixed synapses (no STDP yet)
- Simple readout (logistic regression) from spike counts
- Synthetic audio generator so you can test **without any dataset**

> Goal: Get a **first end-to-end success** in 1–2 hours on your PC. Then we iterate.

## Quickstart

1) **Python 3.11** recommended. Create a venv:
```bash
python -m venv .venv
source .venv/bin/activate  # Windows: .venv\Scripts\activate
```

2) Install dependencies:
```bash
pip install -r requirements.txt
```

3) (Optional) Put your WAV files:
```
neuromorph/
  data/
    yes/  # place some "yes" .wav (16k mono)
    no/   # place some "no"  .wav (16k mono)
```
If you have no audio, the script will **auto-generate synthetic** "yes"/"no" samples.

4) Run the demo:
```bash
python run_demo.py
```

This will:
- Generate (or load) audio
- Convert to event streams
- Run the SNN
- Train a logistic regression on spike counts
- Print accuracy and save a raster plot (`docs/raster_plot.png`).

# Neuromorph SNN (F=48, N=96) — HW/SW Lockstep

## Summary
- Fixed-point LIF SNN (Q1.14), event-driven input
- HW (Verilator) ↔ SW(Q1.14) **match = 1.000000**
- Golden snapshot: `artifacts/golden_spikes.csv`

## Quickstart
```bash
# 0) Build (Verilator)
verilator -sv --binary tb_snn_mem.sv snn_core.sv lif_neuron.sv --top-module tb_snn_mem

# 1) One-shot test (CSV→MEM → HW → SW(Q1.14) → Compare)
make test

# 2) Freeze GOLD
make golden

# 3) Compare HW vs GOLD any time
make compare_golden

# 4) Smoke (single-feature stimuli quick check)
make smoke

# 5) Smoke + compare (HW vs SW(Q1.14) for each)
make smoke_compare
