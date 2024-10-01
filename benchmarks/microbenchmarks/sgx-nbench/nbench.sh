#!/bin/bash
echo "sgx-nbench"

make SGX_MODE=SIM && ./app && make clean