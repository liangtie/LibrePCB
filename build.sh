#!/bin/bash

cmake -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -S . -B build -DUSE_OPENCASCADE=OFF
cmake --build build