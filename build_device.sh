#!/bin/bash

echo Building chipotherm

if [[ $BUILDING_DEVICE -eq 1 ]]; then 
    cd /src
    mkdir -p build_device
    cd build_device
    cmake ..
    make -j4
else 
    DOCKER=$(which podman)

    $DOCKER build -t buzzert/chipotherm-build .

    VERS=$(git describe)

    mkdir -p build_device
    $DOCKER run \
        -v $(dirname "$0"):/src \
        -e BUILDING_DEVICE=1 \
        buzzert/chipotherm-build \
        /src/build_device.sh

    echo Built product \'chipotherm\' in build_device
fi
