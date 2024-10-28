#!/bin/bash

echo "start build script"
echo "current working dir is:"
pwd
echo "its content is:"
ls -l

mkdir build
cd build
echo "current working dir NOW is:"
pwd
cmake ..
make

echo "end build script"