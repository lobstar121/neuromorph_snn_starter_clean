# Dockerfile
FROM ubuntu:24.04

# 기본 도구
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    verilator make g++ python3 python3-pip git ca-certificates && \
    rm -rf /var/lib/apt/lists/*

# python 패키지
RUN python3 -m pip install --upgrade pip && pip install numpy matplotlib

WORKDIR /app
COPY . /app

# 기본값: 테스트 1회
ENV ALPHA_Q14=15520
CMD ["bash", "-lc", "make test && python - << 'PY'\nimport numpy as np,sys\nA=np.loadtxt('artifacts/diff_mask.csv',delimiter=',',dtype=int)\nprint('[Docker] diff sum=',int(A.sum()))\nsys.exit(0 if A.sum()==0 else 2)\nPY"]
