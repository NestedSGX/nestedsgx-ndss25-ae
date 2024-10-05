#!/bin/bash
Test=("A" "D" "F")

Mark="$1"

for i in "${!Test[@]}"; do
    test="${Test[$i]}"
    ./benchmark.sh -w $test "${test}_${Mark}"
done