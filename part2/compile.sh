#!/bin/bash

#Cmake and makefile wrapper
#Previous versions removing
rm -rf build
#Creates a build folder
mkdir build
#Enters 
cd build

#cmake
cmake ..
#make
make

