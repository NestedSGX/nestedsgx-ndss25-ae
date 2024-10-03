#!/bin/bash
outputs="$1"
echo "microbenchmark without the occlum, we will run 'Leaf', edge-call, lmbench, sgx-nbench, wolfssl in one go"

cd Leaf && echo "Leaf Benchmarkings" && ./benchmarks.sh && cd ..

cd microbenchmarks

cd ecall_ocall && ./edgecalls.sh && cd ..

# infact these three outputs are different.
cd lmbench/bw-mem && ./lmbench.sh $outputs && cd ../../

cd sgx-nbench && ./nbnech.sh $outputs && cd ..

cd wolfssl && ./prepare.sh && ./build.sh $outputs

cd ..