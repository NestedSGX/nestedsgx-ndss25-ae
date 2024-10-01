#!/bin/bash
echo "sgx-lmbench"

make SGX_MODE=SIM && ./app && make clean