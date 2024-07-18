#!/bin/zsh

mkdir build
cd build
cmake ..
make
cd ..
current_directory=$(basename "$PWD")
cd build
"./$current_directory"