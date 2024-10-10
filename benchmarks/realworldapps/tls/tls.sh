#!/bin/bash
source /opt/intel/sgxsdk/environment

cd SGX-OpenSSL/OpenSSL_SGX  &&  ./sgx_openssl_setup.sh && make

cd ../SampleCode/Simple_TLS_Server && SGX_MODE=SIM make