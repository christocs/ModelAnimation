#!/usr/bin/env bash

set -x

mkdir build && cd build
/usr/local/bin/cmake -G Ninja -D WarningsAsErrors:BOOL=ON ..
ninja
