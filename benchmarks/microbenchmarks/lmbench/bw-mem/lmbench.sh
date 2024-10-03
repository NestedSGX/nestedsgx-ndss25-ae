#!/bin/bash
echo "sgx-lmbench"

source /opt/intel/sgxsdk/environment && make SGX_MODE=SIM

outputs="$1"

progs=("rd" "wr" "rdwr" "cp" "fwr" "frd" "fcp" "bzero" "bcopy")

for prog in "${progs[@]}"; do
    ./bw_mem -N 10000 2097152 "$prog" | grep -oP '\d+\.\d+(?= MB/SEC)' >> "$outputs"
done

make clean