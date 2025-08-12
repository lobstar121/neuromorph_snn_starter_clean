name: ci

on:
  push:
  pull_request:
  workflow_dispatch:
    inputs:
      run_smoke:
        description: "Run smoke_compare"
        type: boolean
        default: false
      run_sweeps:
        description: "Run alpha/vth/grid sweeps"
        type: boolean
        default: false
      run_learn:
        description: "Run SW-only STDP learn_smoke"
        type: boolean
        default: false
      run_learn_parity:
        description: "Run SW learn + RTL learn + parity"
        type: boolean
        default: false
      commit_sw_hex:
        description: "After parity passes, open PR with weights_learned_sw.hex ([skip ci])"
        type: boolean
        default: false

permissions:
  contents: write         # PR/커밋용
  packages: write

concurrency:
  group: ci-${{ github.ref }}
  cancel-in-progress: false

jobs:
  # --- 기존 기본 테스트 (필요 없으면 삭제 가능) ---
  test:
    runs-on: ubuntu-24.04
    steps:
      - uses: actions/checkout@v4

      - name: Setup Python
        uses: actions/setup-python@v5
        with:
          python-version: "3.12"

      - name: Install deps
        run: |
          sudo apt-get update
          sudo apt-get install -y verilator make g++ python3-pip
          python -m pip install --upgrade pip
          pip install numpy matplotlib

      - name: Clean & build & compare
        env:
          ALPHA_Q14: "15520"
        run: |
          make veryclean || true
          rm -f artifacts/events_ref.mem || true
          make test

      - name: Assert match==1.0
        run: |
          python - << 'PY'
          import numpy as np, sys, os
          p = "artifacts/diff_mask.csv"
          if not os.path.exists(p):
              print("[CI] diff_mask.csv not found"); sys.exit(1)
          A = np.loadtxt(p, delimiter=",", dtype=int)
          s = int(A.sum())
          print(f"[CI] diff sum = {s}")
          sys.exit(0 if s==0 else 2)
          PY

      - name: Upload basic artifacts
        if: always()
        uses: actions/upload-artifact@v4
        with:
          name: basic-artifacts
          path: |
            artifacts/spikes_hw.csv
            artifacts/spikes_sw_q14.csv
            artifacts/diff_mask.csv
            artifacts/diff_heatmap.png
            artifacts/mismatch_*.csv
            artifacts/mismatch_report.txt
          if-no-files-found: ignore

  # --- Docker 이미지 빌드/푸시 (원하면 유지) ---
  docker-image:
    runs-on: ubuntu-24.04
    needs: test
    steps:
      - uses: actions/checkout@v4
      - name: Set up Docker Buildx
        uses: docker/setup-buildx-action@v3
      - name: Log in to GHCR
        if: ${{ github.event_name != 'pull_request' }}
        uses: docker/login-action@v3
        with:
          registry: ghcr.io
          username: ${{ github.actor }}
          password: ${{ secrets.GITHUB_TOKEN }}
      - name: Build & (maybe) Push image
        uses: docker/build-push-action@v6
        with:
          context: .
          file: ./Dockerfile
          push: ${{ github.event_name != 'pull_request' }}
          tags: |
            ghcr.io/${{ github.repository }}:latest
            ghcr.io/${{ github.repository }}:${{ github.sha }}
          labels: |
            org.opencontainers.image.source=${{ github.server_url }}/${{ github.repository }}
            org.opencontainers.image.revision=${{ github.sha }}
      - name: Smoke test inside container
        if: ${{ github.ref == 'refs/heads/main' }}
        run: |
          docker run --rm ghcr.io/${{ github.repository }}:latest \
            bash -lc "make test && python compare_spikes.py artifacts/spikes_hw.csv artifacts/spikes_sw_q14.csv | grep 'match ratio: 1.000000'"

  # --- 수동 스모크 (원하면 유지) ---
  smoke:
    if: ${{ github.event_name == 'workflow_dispatch' && inputs.run_smoke }}
    runs-on: ubuntu-24.04
    steps:
      - uses: actions/checkout@v4
      - uses: actions/setup-python@v5
        with:
          python-version: "3.12"
      - name: Install deps
        run: |
          sudo apt-get update
          sudo apt-get install -y verilator make g++ python3-pip
          pip install numpy
      - name: Run smoke_compare
        env:
          ALPHA_Q14: "15520"
        run: |
          make veryclean || true
          make smoke_compare
      - name: Upload smoke artifacts
        if: always()
        uses: actions/upload-artifact@v4
        with:
          name: smoke-artifacts
          path: artifacts/*X_events_single*.csv

  # --- 수동 스윕 (원하면 유지) ---
  sweeps:
    if: ${{ github.event_name == 'workflow_dispatch' && inputs.run_sweeps }}
    runs-on: ubuntu-24.04
    steps:
      - uses: actions/checkout@v4
      - uses: actions/setup-python@v5
        with:
          python-version: "3.12"
      - name: Install deps
        run: |
          sudo apt-get update
          sudo apt-get install -y verilator make g++ python3-pip
          pip install numpy
      - name: Run alpha/vth/grid sweeps
        env:
          ALPHA_Q14: "15520"
        run: |
          make alpha_sweep
          make vth_sweep
          make grid_sweep
      - name: Upload sweep results
        if: always()
        uses: actions/upload-artifact@v4
        with:
          name: sweep-results
          path: artifacts/*sweep*csv

  # --- SW-only 학습 (원하면 유지) ---
  learn:
    if: ${{ github.event_name == 'workflow_dispatch' && inputs.run_learn }}
    runs-on: ubuntu-24.04
    steps:
      - uses: actions/checkout@v4
      - uses: actions/setup-python@v5
        with:
          python-version: "3.12"
      - name: Install deps (SW-only)
        run: |
          sudo apt-get update
          sudo apt-get install -y make g++ python3-pip
          python -m pip install --upgrade pip
          pip install numpy
      - name: Run SW-only learn_smoke
        env:
          ALPHA_Q14: "15474"
        run: |
          make learn_clean || true
          make learn_smoke
      - name: Upload learn artifacts
        if: always()
        uses: actions/upload-artifact@v4
        with:
          name: learn-artifacts
          path: |
            artifacts/weights_learned.hex
            artifacts/spikes_sw_learn.csv
          if-no-files-found: warn

  # ========= 여기부터 새로 추가: learn parity 파이프라인 =========

  learn_sw:
    if: ${{ github.event_name == 'workflow_dispatch' && inputs.run_learn_parity }}
    runs-on: ubuntu-24.04
    steps:
      - uses: actions/checkout@v4
      - uses: actions/setup-python@v5
        with:
          python-version: "3.12"
      - name: Install deps (SW-only)
        run: |
          sudo apt-get update
          sudo apt-get install -y make g++ python3-pip
          python -m pip install --upgrade pip
          pip install numpy
      - name: Run SW-learn (reference)
        run: |
          make learn_clean || true
          make learn_smoke
          cp -f artifacts/weights_learned.hex artifacts/weights_learned_sw.hex
      - name: Upload SW-learn artifacts
        uses: actions/upload-artifact@v4
        with:
          name: learn-sw
          path: |
            artifacts/weights_learned_sw.hex
            artifacts/spikes_sw_learn.csv

  learn_rtl:
    if: ${{ github.event_name == 'workflow_dispatch' && inputs.run_learn_parity }}
    needs: learn_sw
    runs-on: ubuntu-24.04
    steps:
      - uses: actions/checkout@v4
      - uses: actions/setup-python@v5
        with:
          python-version: "3.12"
      - name: Install deps (RTL)
        run: |
          sudo apt-get update
          sudo apt-get install -y verilator make g++ python3-pip
          pip install numpy
      - name: Run RTL-learn
        run: |
          make veryclean || true
          make learn_rtl
      - name: Upload RTL-learn artifacts
        uses: actions/upload-artifact@v4
        with:
          name: learn-rtl
          path: |
            artifacts/weights_learned_rtl.hex
            artifacts/spikes_hw_learn.csv

  learn_parity:
    if: ${{ github.event_name == 'workflow_dispatch' && inputs.run_learn_parity }}
    needs: [learn_sw, learn_rtl]
    runs-on: ubuntu-24.04
    steps:
      - uses: actions/checkout@v4
      - uses: actions/download-artifact@v4
        with:
          name: learn-sw
          path: sw
      - uses: actions/download-artifact@v4
        with:
          name: learn-rtl
          path: rtl
      - uses: actions/setup-python@v5
        with:
          python-version: "3.12"
      - name: Install deps
        run: |
          python -m pip install --upgrade pip
          pip install numpy
      - name: Parity check (weights + spikes)
        run: |
          python - <<'PY'
          import numpy as np, sys, os
          def read_hex(path):
              xs=[]
              with open(path) as f:
                  for ln in f:
                      ln=ln.strip()
                      if not ln: continue
                      x=int(ln,16)
                      if x & (1<<15): x -= (1<<16)
                      xs.append(x)
              return np.array(xs,dtype=np.int16)

          sw_hex = "sw/weights_learned_sw.hex"
          rtl_hex= "rtl/weights_learned_rtl.hex"
          sw_spk = "sw/spikes_sw_learn.csv"
          rtl_spk= "rtl/spikes_hw_learn.csv"

          if not (os.path.exists(sw_hex) and os.path.exists(rtl_hex)):
              print("[PARITY] missing hex files"); sys.exit(2)
          Wsw = read_hex(sw_hex)
          Wrt = read_hex(rtl_hex)
          if Wsw.shape != Wrt.shape:
              print(f"[PARITY] shape mismatch: {Wsw.shape} vs {Wrt.shape}")
              sys.exit(2)
          w_equal = np.array_equal(Wsw, Wrt)
          print(f"[PARITY] weights equal: {w_equal}")

          # spikes optional but if present, require exact match
          if os.path.exists(sw_spk) and os.path.exists(rtl_spk):
              Ssw = np.loadtxt(sw_spk, delimiter=",", dtype=int)
              Srt = np.loadtxt(rtl_spk, delimiter=",", dtype=int)
              if Ssw.shape != Srt.shape:
                  print(f"[PARITY] spike shape mismatch: {Ssw.shape} vs {Srt.shape}")
                  sys.exit(2)
              s_equal = np.array_equal(Ssw, Srt)
              print(f"[PARITY] spikes equal: {s_equal}")
              ok = w_equal and s_equal
          else:
              ok = w_equal

          sys.exit(0 if ok else 2)
          PY

      # --- (옵션) SW hex를 PR로 반영 ---
      - name: Copy SW hex into repo (artifacts/weights_learned_sw.hex)
        if: ${{ inputs.commit_sw_hex }}
        run: |
          mkdir -p artifacts
          cp -f sw/weights_learned_sw.hex artifacts/weights_learned_sw.hex

      - name: Create PR with weights_learned_sw.hex
        if: ${{ inputs.commit_sw_hex }}
        uses: peter-evans/create-pull-request@v6
        with:
          commit-message: "Add weights_learned_sw.hex [skip ci]"
          title: "Update: weights_learned_sw.hex"
          body: |
            This PR updates the SW reference learned weights produced by CI parity run.
            - Generated by workflow_dispatch run
            - Includes [skip ci] to avoid recursive runs
          branch: ci/update-sw-hex
          add-paths: |
            artifacts/weights_learned_sw.hex
