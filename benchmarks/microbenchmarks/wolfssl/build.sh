outputs="$1"
echo "build the environment: options.h"
cd  wolfssl-5.7.0-stable 
./autogen.sh && ./configure && ./config.status
cd IDE/LINUX-SGX

echo "build the static library"
./build.sh

cd ../../wolfssl-examples/SGX_Linux
./build.sh ../../wolfssl-5.7.0-stable/
./App -b > $outputs