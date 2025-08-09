import numpy as np

class LIFCore:
    """
    Discrete-time LIF SNN core with dense connections.
    - Neurons: N
    - Inputs:  F (feature channels per timestep)
    - We map input events x[t, F] -> spikes via W_in (F x N)
    - Recurrent W_rec is optional (disabled by default for simplicity)
    """
    def __init__(self, n_inputs: int, n_neurons: int, dt=0.001,
                 tau_m=0.02, v_th=1.0, v_reset=0.0, refrac=0.002,
                 w_in_scale=0.5, recurrent=False, seed=42):
        rng = np.random.default_rng(seed)
        self.F = n_inputs
        self.N = n_neurons
        self.dt = dt
        self.tau_m = tau_m
        self.alpha = np.exp(-dt / tau_m)
        self.v_th = v_th
        self.v_reset = v_reset
        self.refrac_steps = int(refrac / dt)
        self.recurrent = recurrent

        # Input weights
        self.W_in = rng.normal(0, w_in_scale / np.sqrt(n_inputs), size=(n_inputs, n_neurons)).astype(np.float32)
        # Optional recurrent (small)
        self.W_rec = None
        if recurrent:
            self.W_rec = rng.normal(0, 0.05 / np.sqrt(n_neurons), size=(n_neurons, n_neurons)).astype(np.float32)
            np.fill_diagonal(self.W_rec, 0.0)

    def forward(self, X_events: np.ndarray):
        """
        X_events: (T, F) binary/float event stream
        Returns:
          spikes: (T, N) binary spikes
          V:      (T, N) membrane potentials
        """
        T, F = X_events.shape
        assert F == self.F, f"Expected F={self.F}, got {F}"
        V = np.zeros((T, self.N), dtype=np.float32)
        spikes = np.zeros((T, self.N), dtype=np.uint8)
        refrac_cnt = np.zeros((self.N,), dtype=np.int32)

        for t in range(T):
            # Leak
            if t > 0:
                V[t] = self.alpha * V[t-1]

            # Input current
            I_in = X_events[t].astype(np.float32) @ self.W_in  # shape (N,)

            # Recurrent
            if self.recurrent and t > 0:
                I_rec = spikes[t-1].astype(np.float32) @ self.W_rec
            else:
                I_rec = 0.0

            V[t] += I_in + I_rec

            # Refractory handling
            active = refrac_cnt <= 0
            fired = (V[t] >= self.v_th) & active
            spikes[t, fired] = 1
            # reset
            V[t, fired] = self.v_reset
            # set refractory
            refrac_cnt[fired] = self.refrac_steps
            refrac_cnt[~fired] = np.maximum(0, refrac_cnt[~fired] - 1)

        return spikes, V
