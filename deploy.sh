#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: deploy.sh [chip ip address]"
    echo "Make sure to build with arm toolchain first"
    exit 1
fi

scp chipotherm "root@$1:~/chipotherm" && ssh -t root@$1 "DISPLAY=:0 ~/chipotherm"

