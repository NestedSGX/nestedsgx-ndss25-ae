#!/bin/bash
echo "set occlum environment for NestedSGX"

# download the 0.29.7 version of occlum
git clone -b 0.29.7 https://github.com/occlum/occlum.git

# patch the occlum with the patches.
cd occlum
git am ../occlum-patches.patch

curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
rustup target add x86_64-unknown-linux-musl
# build the occlum.
make submodule && SGX_MODE=SIM make && make install 