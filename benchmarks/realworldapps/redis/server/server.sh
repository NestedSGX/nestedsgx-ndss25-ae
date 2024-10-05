#!/bin/bash
source /opt/intel/sgxsdk/environment
SGX_MODE=SIM ./prepare.sh
SGX_MODE=SIM ./build.sh
SGX_MODE=SIM ./run.sh