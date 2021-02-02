#!/bin/bash

# -e: exit on command error
# -u: error on variables not set 
set -eou pipefail

# Reading arguments
testing=$1

# Set /usr/local/lib in the library path
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

# Create build folder
mkdir -p build && cd build

# Configure: Unix Makefiles, Debug enables testing and code coverage
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Build (for Make on Unix equivalent to `make -j $(nproc)`)
#cmake --build . --config Debug -- -j8
make -j8

# Whether perform testing or not
if [[ $testing = "testing" ]]; then
    # Test
    #cd ../tests
    #python3 compile.py 
    #LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH ctest --output-on-failure
    #LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH OMP_NUM_THREADS=1 ctest
    #--rerun-failed
    make test
fi
