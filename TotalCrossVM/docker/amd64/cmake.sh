#!/bin/bash
BASEDIR=$(dirname $0)
WORKDIR=$(cd $BASEDIR; pwd)

sudo rm -Rf bin
mkdir build

# execute docker run
sudo docker run -v ${WORKDIR}/../docker/amd64/build:/build \
                -v ${WORKDIR}/../:/sources \
                -t totalcross/linux-amd64-build bash -c "cmake ../sources && cmake --build ."