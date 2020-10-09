#!/bin/sh

echo "Original..."
./dump_arrays_$1_original.o 2> __tmp_file;
echo "SIMD version..."
./dump_arrays_$1.o 2> __tmp_file2;

diff __tmp_file __tmp_file2

#rm __tmp*
