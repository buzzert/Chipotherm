#!/bin/bash

echo Building chipotherm

docker build -t buzzert/chipotherm:build .

VERS=$(git describe)

mkdir -p build_device
docker container create --name extract buzzert/chipotherm:build
docker container cp extract:/build/src/chipotherm-$VERS ./build_device
docker container rm -f extract

echo Built product \'chipotherm\' in build_device

