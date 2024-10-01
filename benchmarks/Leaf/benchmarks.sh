#!/bin/bash
source /opt/intel/sgxsdk/environment
echo "EGETKEY"

cd EGETKEY && make SGX_MODE=SIM && ./app && make clean

cd ..

echo "EREPORT"

cd EREPORT && make SGX_MODE=SIM && ./app && make clean

cd ..

make bench && make clean