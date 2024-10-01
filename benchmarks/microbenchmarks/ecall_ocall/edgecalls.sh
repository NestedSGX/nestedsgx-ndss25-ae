#!/bin/bash
echo "edge_call"

source /opt/intel/sgxsdk/environment && make SGX_MODE=SIM && ./app && make clean
