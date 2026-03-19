#!/bin/bash

mkdir -p build
cd build || exit 1

cmake ..
cmake --build .

./lab_memory_manager
