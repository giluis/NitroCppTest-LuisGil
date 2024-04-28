FROM ubuntu:24.04

ARG DEBIAN_FRONTEND=noninteractive

# Install Boost deps
RUN apt-get update && apt-get install -y \
    build-essential \
    libboost-all-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY main.cpp .

# Run the executable
CMD ["tail", "-f", "/dev/null"]


