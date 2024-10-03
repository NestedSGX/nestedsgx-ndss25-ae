echo "download wolfssl"
git clone https://github.com/wolfSSL/wolfssl-examples.git
cd wolfssl-examples && git am ../diff.patch && cd ..
wget https://github.com/wolfSSL/wolfssl/archive/refs/tags/v5.7.0-stable.tar.gz
tar -zxvf v5.7.0-stable.tar.gz