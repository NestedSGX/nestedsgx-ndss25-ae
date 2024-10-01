#!/bin/bash
echo "microbenchmark without the occlum, we will run 'Leaf', edge-call, lmbench, sgx-nbench, wolfssl in one go"

cd Leaf && echo "Leaf Benchmarkings" && ./benchmarks.sh && cd ..

cd microbenchmarks

cd ecall_ocall && ./edgecalls.sh && cd ..

cd lmbench && ./lmbench.sh && cd ..

cd sgx-nbench && ./nbnech.sh && cd ..

cd ..