FROM resin/armv7hf-debian-qemu
 
RUN apt-get update;\
    apt-get install -y \
        build-essential \
        pkg-config \
        cmake \
        libx11-dev libboost-dev libhidapi-dev libcairo2-dev \
        libsoup2.4-dev libpango1.0-dev libxfixes-dev

COPY . /chipotherm
WORKDIR /chipotherm

RUN mkdir chip_build 
WORKDIR chip_build
 
RUN cmake ../ && make

