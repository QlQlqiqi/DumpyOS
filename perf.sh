#!/bin/bash


sudo perf record -F 999 -g $1 $2
sudo perf script -i perf.data &> ~/FlameGraph/perf.unfold
pushd ~/FlameGraph
./stackcollapse-perf.pl perf.unfold &> perf.folded
./flamegraph.pl perf.folded > perf.svg
popd

