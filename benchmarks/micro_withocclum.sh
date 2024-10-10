#!/bin/bash
outputs="$1"
echo "Test on FIO"
cd microbenchmarks/fio && ./fio.sh $outputs
