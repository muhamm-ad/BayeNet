#!/bin/bash

# Exit if any command fails
set -e

workspace=$(pwd)

cd ${workspace}

# If the build directory exists, delete it
if [ -d "build" ]; then
  rm -r build
fi

# Create the build directory
mkdir -p build
cd build
cmake ..
make -j$(nproc)
