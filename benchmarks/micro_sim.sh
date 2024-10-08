#!/bin/bash
outputs="$1"
echo "microbenchmark of SIM mode, we will run lmbench, sgx-nbench, wolfssl in one go"

# infact these three outputs are different.
cd microbenchmarks

cd lmbench/bw-mem && ./lmbench.sh $outputs && cd ../../

cd sgx-nbench && ./nbench.sh $outputs && cd ..

cd wolfssl && ./prepare.sh && ./build.sh $outputs

cd ..