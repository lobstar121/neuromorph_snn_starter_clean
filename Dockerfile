# ---- base
FROM ubuntu:24.04

# OS deps + Python/Verilator toolchain + venv
RUN apt-get update && DEBIAN_FRONTEND=noninteractive \
    apt-get install -y --no-install-recommends \
      python3 python3-pip python3-venv make g++ verilator ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# (선택) requirements.txt 없으면 기본 생성
COPY requirements.txt /app/requirements.txt
RUN test -s /app/requirements.txt || printf "numpy\nmatplotlib\n" > /app/requirements.txt

# ---- venv 만들고 의존성 설치
RUN python3 -m venv /opt/venv \
 && . /opt/venv/bin/activate \
 && pip install --upgrade pip \
 && pip install --no-cache-dir -r /app/requirements.txt

# venv PATH 사용
ENV PATH="/opt/venv/bin:${PATH}"

# 소스 복사
COPY . /app

# 기본 실행(원하면 수정)
CMD ["bash", "-lc", "make test && python compare_spikes.py artifacts/spikes_hw.csv artifacts/spikes_sw_q14.csv | grep 'match ratio: 1.000000'"]
