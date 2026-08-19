FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    make \
    gcc \
    g++ \
    clang-15 \
    lld-15 \
    python3 \
    python3-pyelftools \
    python3-pip \
    wget \
    git \
    && ln -sf /usr/bin/clang-15 /usr/bin/clang \
    && ln -sf /usr/bin/clang++-15 /usr/bin/clang++ \
    && ln -sf /usr/bin/lld-15 /usr/bin/lld \
    && ln -sf /usr/bin/lld-15 /usr/bin/ld.lld \
    && rm -rf /var/lib/apt/lists/*

# Clone and build PS5-Payload-SDK
WORKDIR /usr/src
RUN git clone https://github.com/john-tornblom/ps5-payload-sdk.git
WORKDIR /usr/src/ps5-payload-sdk
RUN make && make DESTDIR=/opt/ps5-payload-sdk install

ENV PS5_PAYLOAD_SDK=/opt/ps5-payload-sdk

# Default command is to make the payload in the mounted volume
WORKDIR /app
CMD ["make"]
