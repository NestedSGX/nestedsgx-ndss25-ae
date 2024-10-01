#!/bin/bash
echo "edge_call"

make SGX_MODE=SIM && ./app && make clean
