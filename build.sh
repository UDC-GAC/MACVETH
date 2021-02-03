#!/bin/bash

# -e: exit on command error
set -eo pipefail

# Reading arguments
testing=$1

# Set /usr/local/lib in the library path
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

# Create build folder
mkdir -p build && cd build

# Configure: Unix Makefiles, Debug enables testing and code coverage
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Build (for Make on Unix equivalent to `make -j $(nproc)`)
# using cmake: cmake --build . --config Debug -- -j8
make -j8

# Whether perform testing or not
if [[ $testing == "testing" ]]; then
    # Testing is performed using CTest, taking full advantage of CMake suite.
    # Therefore, testing can be called either using Makefile or ctest. For
    # simplicity, I think using Makefile is better, as there are no input
    # parameters, only make test. Anyways, ctest could be done using something like:
    # OMP_NUM_THREADS=1 ctest --rerun-failed
    make test
fi
