FROM ubuntu:24.04

# noninteractive apt
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y \
    verilator make g++ python3 python3-pip git \
 && rm -rf /var/lib/apt/lists/*

# workdir
WORKDIR /app
COPY . /app

# python deps
RUN python3 -m pip install --upgrade pip && pip3 install -r requirements.txt

# default env
ENV ALPHA_Q14=15520

# quick self-test as default command (can be overridden)
CMD bash -lc "make veryclean || true && make test && python compare_spikes.py artifacts/spikes_hw.csv artifacts/spikes_sw_q14.csv && echo 'OK'"
