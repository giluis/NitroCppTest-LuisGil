FROM ubuntu:24.04

ARG DEBIAN_FRONTEND=noninteractive

# Install Boost deps
RUN apt-get update && apt-get install -y \
    build-essential \
    # at time of writing, 1.71.0
    libboost-all-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . /app

RUN make 

ENTRYPOINT ["./main"]

# Run the executable
CMD ["example.json"]


