#!/bin/bash -e

rm -rf build
mkdir build
cd build
cmake \
-DCMAKE_BUILD_TYPE=Debug \
-DBUILD_STATIC_EXECUTABLES=FALSE \
-DBUILD_SHARED_LIBS=FALSE \
..
#make VERBOSE=1 install
make install
