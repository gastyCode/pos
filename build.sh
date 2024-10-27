#!/bin/bash

mkdir build
cd build
cmake ..

cd sll-example
make
cd ..

cd graph-example
make
cd ..
