#!/bin/sh

# Intel Xeon E5 v4 (Broadwell)
rm -R results_broadwell/$1
mkdir -p results_broadwell/
scp -r marcos.horro@pluton.des.udc.es:workspace/MACVETH/tests/perf_test/results/$1 results_broadwell/$1
# AMD Threadripper (Zen)
rm -R results_zen/$1
mkdir -p results_zen/
scp -r horro@193.144.50.143:workspace/MACVETH/tests/perf_test/results/$1 results_zen/$1