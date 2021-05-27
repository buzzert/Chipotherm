FROM resin/armv7hf-debian-qemu

# RUN printf "deb http://archive.debian.org/debian jessie main\n" > /etc/apt/sources.list
# RUN printf "deb-src http://archive.debian.org/debian jessie main\n" >> /etc/apt/sources.list
 
RUN apt-get update;\
    apt-get install -y \
        build-essential \
        pkg-config \
        cmake \
        git \
        libx11-dev libboost-dev libhidapi-dev libcairo2-dev \
        libsoup2.4-dev libpango1.0-dev libxfixes-dev libcanberra-dev libudev-dev

