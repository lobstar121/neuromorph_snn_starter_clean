# 베이스
FROM ubuntu:24.04

# OS deps
RUN apt-get update && DEBIAN_FRONTEND=noninteractive \
    apt-get install -y --no-install-recommends \
      python3 python3-pip make g++ verilator ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# 작업 디렉터리
WORKDIR /app

# 파이썬 deps: 먼저 requirements만 복사 → 캐시 활용
COPY requirements.txt /app/requirements.txt
RUN python3 -m pip install --upgrade pip \
 && pip3 install --no-cache-dir -r /app/requirements.txt

# 소스 복사
COPY . /app

# 기본 환경
ENV ALPHA_Q14=15520

# 컨테이너 디폴트 엔트리포인트
CMD ["bash"]
