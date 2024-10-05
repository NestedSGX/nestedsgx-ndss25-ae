## Benchmarking
NestedSGX provides many benchmarks that can be studied and compared to other SGX-like System. To run the benchmarks, please follow the steps below:

### Preparation
We encapsulate the Benchmarks with several different scripts, however as you fetch the project from github, this might not be able to run easily, to enable all of the scripts, please first entitle the scripts with the execution priviledges.
```
find "." -name "*.sh" -exec chmod +x {} \;
```
### Microbenchmarks without Occlum
Several microbenchmarks can be run so long as you have intel-sgx environment correctly installed. Make sure you've installed it in `/opt/intel/sgxsdk` beforehand. When finished, you can run `micro_withoutocclum.sh`, which includes the following benchmarks.
- Leaf instructions of Intel SGX: which demostrates the performance of SGX leaf instructions. (Leaf)
- Edge Call: which measures the time to switch world between the APP and Enclave. (microbenchmarks/ecall_ocall)
- Lmbench: evaluate the memory bandwidth. (microbenchmarks/lmbench)
- sgx-nbench: CPU, FPU, memory system benchmarks. (microbenchmarks/sgx-nbench)
- Wolfssl: computation-intensive tasks.

To plot the graph, results of the last three benchmarks should be kept in Files.
```
./micro_withoutocclum.sh NSGX.txt
```
Which will automatically store the result of Lmbench, sgx-nbench, wolfssl in `NSGX.txt` in their own directory.

On the other hand, to compare the results with SIM mode of SGX, we provide another version of benchmarks, named `micro_sim.sh`: please make sure you've switched to original version of linux-sgx-sdk when running this benchmarks.
```
./micro_sim.sh BASE.txt
```
### Macrobenchmarks without Occlum
Serveral other macrobenchmarks can be obtained automatically through `macro_withoutocclum.sh`.
- hashjoin: another computation-intensive task.
- sqlite-native: SGX-version of SQlite.

Still we should record the performance in files.
```
./macro_withoutocclum.sh NSGX.txt
```
### With Occlum:
We support Redis/FIO with Occlum, however, this is not a easy work. We won't provide the automatical benchmarks for setting things up in our AE repo, please follow our steps below carefully.

In our docker environment, please use `docker ps -a` to check the given container environment that use occlum/ubuntu20.04. You can use `docker start xxxx` and `docker exec -it xxxx /bin/bash` to start and open the terminal of the container.

