FROM gcc:14.2.0-bookworm as build

RUN apt-get update && apt-get install -y \
    cmake \
    zlib1g-dev \
    libjpeg-dev \
    libexpat1-dev \
    && rm -rf /var/lib/apt/lists/*

# CMD ["bash"]