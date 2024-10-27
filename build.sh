#!/bin/bash

mkdir build
cd build
cmake ..

cd sll-tests
make
cd ..

cd sll-example
make
cd ..

cd graph-example
make
cd ..
