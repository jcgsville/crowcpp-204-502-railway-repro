FROM ubuntu:24.04 AS builder
ARG TARGETARCH

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    # Needed for network calls to fetch dependencies
    ca-certificates \
    cmake \
    git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY CMakeLists.txt ./
COPY server.cpp ./server.cpp

RUN cmake -S . -B .build -DCMAKE_BUILD_TYPE=Release
RUN cmake --build .build --target server --parallel $(nproc)

FROM ubuntu:24.04 AS app

WORKDIR /app

COPY --from=builder /app/.build/server /usr/local/bin/server

EXPOSE ${PORT}

ENTRYPOINT ["/usr/local/bin/server"]
