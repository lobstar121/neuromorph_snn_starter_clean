import os
import glob
import numpy as np
import librosa

def load_wavs_or_generate(base_dir, sr=16000, n_per_class=20, seed=0):
    yes_dir = os.path.join(base_dir, "yes")
    no_dir = os.path.join(base_dir, "no")
    yes_files = sorted(glob.glob(os.path.join(yes_dir, "*.wav")))
    no_files = sorted(glob.glob(os.path.join(no_dir, "*.wav")))

    ys, ns = [], []
    # Load if present
    for p in yes_files[:n_per_class]:
        y, _ = librosa.load(p, sr=sr, mono=True)
        ys.append(y)
    for p in no_files[:n_per_class]:
        y, _ = librosa.load(p, sr=sr, mono=True)
        ns.append(y)

    # If not enough, synthesize simple chirps to emulate two classes
    rng = np.random.default_rng(seed)
    def synth_word(kind):
        dur = rng.uniform(0.6, 0.9)
        N = int(sr*dur)
        t = np.linspace(0, dur, N, endpoint=False)
        if kind == "yes":
            # rising chirp
            f0, f1 = 300, 1200
        else:
            # falling chirp
            f0, f1 = 1200, 300
        k = (f1 - f0) / dur
        phi = 2*np.pi*(f0*t + 0.5*k*t**2)
        sig = 0.5*np.sin(phi)
        # random formant-like AM
        sig *= (0.7 + 0.3*np.sin(2*np.pi*5*t))
        # small noise
        sig += 0.02*rng.standard_normal(N)
        return sig.astype(np.float32)

    while len(ys) < n_per_class:
        ys.append(synth_word("yes"))
    while len(ns) < n_per_class:
        ns.append(synth_word("no"))

    X = ys + ns
    y = np.array([1]*len(ys) + [0]*len(ns), dtype=np.int64)
    return X, y, sr

def mel_bands(x, sr, n_mels=24, hop=128):
    S = librosa.feature.melspectrogram(y=x, sr=sr, n_mels=n_mels, hop_length=hop, n_fft=512, power=2.0)
    S_db = librosa.power_to_db(S + 1e-10)
    S_db = (S_db - S_db.min()) / (S_db.max() - S_db.min() + 1e-9)
    return S_db.T  # (T, n_mels)

def to_events_from_mel(M, thresh=0.15):
    d = np.diff(M, axis=0, prepend=M[:1])
    on  = (d >  thresh).astype(np.uint8)
    off = (d < -thresh).astype(np.uint8)
    return np.concatenate([on, off], axis=1)

def build_event_dataset(base_dir, sr=16000, n_per_class=20, n_mels=24, hop=128, thresh=0.15):
    wavs, y, sr = load_wavs_or_generate(base_dir, sr=sr, n_per_class=n_per_class)
    X_events = []
    lengths = []
    for w in wavs:
        M = mel_bands(w, sr=sr, n_mels=n_mels, hop=hop)
        E = to_events_from_mel(M, thresh=thresh)
        X_events.append(E)
        lengths.append(E.shape[0])
    # Pad to same T for batch processing (use min length)
    T = min(lengths)
    X_events = [E[:T] for E in X_events]
    X_events = np.stack(X_events, axis=0)  # (B, T, F)
    return X_events, y, {"T": T, "F": X_events.shape[-1], "sr": sr, "hop": hop}
