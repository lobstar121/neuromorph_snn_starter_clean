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

