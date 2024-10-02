echo "download wolfssl"
git clone https://github.com/wolfSSL/wolfssl-examples.git
cd wolfssl-examples && git am ../diff.patch
wget https://github.com/wolfSSL/wolfssl/archive/refs/tags/v5.7.0-stable.tar.gz
tar -zxvf v5.7.0-stable.tar.gz

echo "build the environment: options.h"
cd  wolfssl-5.7.0-stable 
./autogen.sh && ./configure && ./config.status
cd IDE/LINUX_SGX

echo "build the static library"
./build.sh

cd ../../wolfssl-examples/SGX_Linux
./build.sh ../../wolfssl-5.7.0-stable/