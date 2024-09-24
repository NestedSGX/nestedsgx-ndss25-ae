## Some notes on running microbenchmarks.
for lmbench/sgx-nbench/wolfssl:
- what you need is simply set up our customized `sgx-sdk` and run these benchmarks in SIM mode. Make sure you have the `svsm-guest-occlum` kernel module `insmod`ed.

for fio:
- this benchmark relies on the LibOS named `Occlum` by Ant Group, make sure you've install our `sgx-sdk` and `Occlum` beforehand.
- to get the same performance in fio, you should build according to `hotcall`, i.e. shift your `sgx-sdk` and `occlum` version to the ones in `hotcall`.
- else the performance of the `fio-read-seq` won't be so great according to great cost in `world-switching`.
- the performance we've shown in the paper is the version of `Hotcall`, at the cost of stablity (Our implementation of `Hotcall` is a little buggy, and might fail to work sometimes, so `reboot` the system if it happens)