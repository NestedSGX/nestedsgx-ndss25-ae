## Benchmarking
NestedSGX provides many benchmarks that can be studied and compared to other SGX-like System. To run the benchmarks, please follow the steps below:

### Preparation
We encapsulate the Benchmarks with several different scripts, however as you fetch the project from github, this might not be able to run easily, to enable all of the scripts, please first entitle the scripts with the execution priviledges.
```
find "." -name "*.sh" -exec chmod +x {} \;
```
### Microbenchmarks without Occlum
Several microbenchmarks can be run so long as you have intel-sgx environment correctly installed. Make sure you've installed it in `/opt/intel/sgxsdk` beforehand. 

To make it easier, some tips are mentioned below:
```shell
# download linux-sgx, follow the steps in linux-sgx to download necessary software in your environment, run make preparation
make clean && make sdk_install_pkg_no_mitigation` # to get the `binary` setup package.

# after that you can get this binary in linux/installer/bin/
./sgx_linux_x64_sdk_2.20.100.4.bin 

# to install sgx-sdk, make sure you always install it in `/opt/intel`, after that, use `source /opt/intel/sgxsdk/environment` to apply it in the OS.
```
When finished, you can run `micro_withoutocclum.sh`, which includes the following benchmarks.
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

We will substitute the occlum with our own version, `set_occlum.sh` will automatically install our modified occlum for us.

After that, we can launch our tests on FIO and Redis. 

- FIO: in microbenchmarks/fio, this can be done by simply calling `./fio.sh NSGX.txt`, it might take a while, so please wait patiently.

On the other hand, Redis might involve some manual work, see `README.md` in `realworldapps/redis`. You should first set up the server environment, and then in another terminal, run `./benchmarks.sh NSGX.txt` to get the timing latency we need.

As a reference, We test our benchmarks in following steps.
```shell
# on server side. This will launch the redis server based on NestedSGX.
./server.sh

# launch another terminal for the clinet side.
docker exec -it <CONTAINER ID> /bin/bash
# in client directory
./benchmarks.sh <TAG> (e.g. NSGX)
```
### Hotcall for FIO/REDIS
We provide another version of Occlum and Intel-SGX-SDK to imporve the performance of FIO and Redis, however, this attempt might not be so stable, which means we might fail to run our benchmarks and NestedSGX system might crash. If crash, please reboot the guest machine. (kill the qemu process and reboot).
```shell
ps aux | grep qemu # Get the process id.
kill -9 id_0, id_1 # kill the invovled process.
```

The only way you need to do is to replace occlum and intel-sgx-sdk to hotcall version, please refer to `README.md`in the root directory and download the Hotcall version.

And then, repeat the last section "With Occlum" and see whether the performance of these two benchmarks is improved.
### Manual benchmark
We still have some bug and limitation in NestedSGX, the `tlserver` in `realworldapps/tls` is not able to launch smoothly. Even though we can get the handshake times as we expect, when all the threads exit in `pthread.join` step, this app will crash. We hope in the future we can aid this obstacle, though it is now sufficient to see the performance differences between NestedSGX and SIM mode of SGX.

Please refer to the `README.md` in `tls` if you want to have a look.

### Plotting:
Happy to see you here in plotting section!! In fact, the benchmarking section of NestedSGX is to some extent too long. 

However, to have a clearer look at the differences between SIM mode and NestedSGX mode performance, we still need several things to do.

We will provide `plot` files in every benchmarks we provide (When comparison is highly invovled). To get the SIM mode version of performance benchmarks, shift back to the original version of Occlum, and run the above benchmarks again, remember to record all the data in another name (i.e. not the `NSGX` we used before), or this might override the data we've obtained before.

In the docker image, run the following instructions.
```shell
./micro_sim.sh SIM
./macro_withoutocclum.sh SIM

# Then run FIO/REDIS/TLS in the same way.

```
<!-- 2fec098cb3c7 for SIM and 7d0b131e4758 for NSGX -->
When we get another version of result of benchmarks (e.g. `SIM` data), we can carefully plot the results.

In every benchmark, we provide the corresponding plot python scripts, this might not run smoothly in the Guest VM, and maybe you can't directly have a look at the output `*.pdf` file as you wish, so you can use `scp` to transfer it into the Host OS, and draw the graph in the Host OS machine.
```shell
scp -r [your_path_to_the_nestedsgx]/benchmarks ae-ndss25@[your_host_machine_ip]:[your_path_in_the_host_machine] .
```
If you follow our steps above, in these benchmark dirents, you'll have `NSGX.txt` as the result of the NestedSGX version of benchmark performance, and `SIM.txt` as the result of the simulation version of benchmark performance. The `*.pdf` file for the comparsion can be obtained using the following scripts.
```shell
# Taking microbenchmarks/sgx-nbench/ as example

python nbench.py -nsgx NSGX.txt -sim SIM.txt

# This will output a file named nbench.pdf, and you can check the pdf using plug-ins in VSCODE.
```