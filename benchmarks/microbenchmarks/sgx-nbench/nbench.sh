#!/bin/bash
echo "sgx-nbench"

source /opt/intel/sgxsdk/environment && make SGX_MODE=SIM && ./app && make clean