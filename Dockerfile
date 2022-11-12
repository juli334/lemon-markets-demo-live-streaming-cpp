### Build stage ###

FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND noninteractive

WORKDIR /usr/src/app

COPY . .

# Install build tools and libraries
RUN apt update -y && \
    apt install -y \
        build-essential \
        cmake \
        libcurl4-nss-dev \
        libpaho-mqtt-dev

# Build the project
RUN mkdir build && \
    cd build && \
    cmake .. && \
    cmake --build .


### Run stage ###

FROM ubuntu:22.04

WORKDIR /usr/src/app

# Copy the built executable to the current stage
COPY --from=builder /usr/src/app/build/stream ./

# Copy the dependencies of the executable to the current stage
COPY --from=builder /usr/lib/x86_64-linux-gnu/ /usr/lib/x86_64-linux-gnu/

# Run the executable
CMD [ "./stream" ]