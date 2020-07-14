#!/bin/sh

make -B COUNTER=TIME N=$1 TS=$2 DTYPE=$3

FLOPS=$((1*$1*$1*$2))

echo "Results for only one reduction:"
echo "-> original..."
sh time_benchmark.sh ./reductions_original.o $FLOPS
echo "-> not fused..."
sh time_benchmark.sh ./reductions.o $FLOPS

FLOPS=$(($FLOPS*2))

#echo ""
#echo ""
echo "Results for only two reductions:"
echo "-> original..."
sh time_benchmark.sh ./reductions2_original.o $FLOPS
echo "-> fused..."
sh time_benchmark.sh ./reductions2_fused.o $FLOPS
echo "-> not fused..."
sh time_benchmark.sh ./reductions2.o $FLOPS

