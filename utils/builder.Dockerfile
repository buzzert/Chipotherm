FROM resin/armv7hf-debian-qemu

RUN echo 'deb http://deb.debian.org/debian stretch-backports main' > \
    /etc/apt/sources.list.d/backports.list
 
RUN apt-get update;\
    apt-get install -y \
        build-essential; \
    apt-get install -t stretch-backports meson ninja-build
 
WORKDIR /pwd
ENTRYPOINT ["/usr/bin/meson"]

