#!/bin/bash
mkdir build
cd build
cmake ..
make
cp ./TEST ../TEST
cd  ../
rm -rf ./build