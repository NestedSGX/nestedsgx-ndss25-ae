#!/bin/bash
echo "sgx-nbench"
outputs="$1"

source /opt/intel/sgxsdk/environment && make SGX_MODE=SIM && ./app > $outputs
make clean