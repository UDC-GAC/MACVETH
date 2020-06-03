#!/bin/bash

set -eo pipefail

# Reading arguments
testing=$1

# Set /usr/local/lib in the library path
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

# Create build folder
mkdir -p build && cd build

# Configure
cmake -DCODE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug ..

# Build (for Make on Unix equivalent to `make -j $(nproc)`)
cmake --build . --config Debug -- -j2

# Whether perform testing or not
if [[ $testing = "testing" ]]; then
    # Test
    LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH ctest --output-on-failure
    LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH OMP_NUM_THREADS=1 ctest --rerun-failed
fi
