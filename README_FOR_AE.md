## A step-by-step README
We are sorry the different README.md files have caused your confusion, so we decide to write another one for you.

### Step 1: Entering the Guest OS
```
sudo ./launch-qemu.sh -hda ae.qcow2 -sev-snp -svsm svsm.bin
```
That way you can start the Guest VM. However, the display of the default Guest VM might be too small (i.e. 640 x 480), so if you want to better your use experience. We recommend you try to access the Guest VM using SSH.
```
ssh -p 30001 ae-ndss@localhost
```
**For simplicity, we recommend you turn yourself the root user before running all the following commands, this will ease a lot of headaches.**
```
sudo su
```
### Step 2: Choose what mode you want to run
To compare the performance of NestedSGX, we pre-downloaded the different version of components in the Guest OS, which can be found in `components` directory.

Three version of components are provided.
- nsgx: the NestedSGX version.
- sim: the Simulation version (We choose it as the Baseline).
- Hotcall: the accelerated NestedSGX version using Hotcall methods (ISCA'2017)

For simplicity, we denote the mode you want to run as `[mode]` in the following steps.

If you want to choose `sim` as the `[mode]`, that means the benchmarks don't rely on the Guest Kernel Module, please jump to **Step 4**.

### Step 3: Install the Kernel Module of NestedSGX
So, you've chosen the `nsgx` or `Hotcall` as the mode you want to run now. If you want to run benchmarks in these two modes, installing the Guest Kernel Module of NestedSGX is a must.

You can find a directory named `svsm-guest-occlum` in `nestedsgx-ndss25-ae`, which contains the Kernel Module above.

Please follow the following steps to install this module. Make sure you are a root user first.
```
cd svsm-guest-occlum
make
insmod svsm_guest.ko
```
This Kernel Module will modify the page table of the Guest OS and SVSM, **but sometimes it might fail**, so we dump some messages in the Kernel to make sure whether the modification is done successfully.

Typeing the following command to see whether the correctness.
```
dmesg
```
If you see something like below, i.e, **all of the code page trampoline and the data page trampoline succeed**, this means the Guest Kernel Module is done right.
```
[22349.790649] VS_GUEST: print_page_table_entry: 34. VS_INFO. belonging of pagetable walk: insmod
[22349.791636] VS_GUEST: print_page_table_entry: 65. VS_INFO. Page table entry at virtual address ffff9defcf932000:
[22349.795227] VS_GUEST: print_page_table_entry: 66. VS_INFO.   PTE Value: 800010f932163
[22349.796536] VS_GUEST: judge: 10. VS_INFO. code page trampoline succeed.
[22349.797688] VS_GUEST: print_page_table_entry: 34. VS_INFO. belonging of pagetable walk: insmod
[22349.797691] VS_GUEST: print_page_table_entry: 65. VS_INFO. Page table entry at virtual address ffff9defcf933000:
[22349.800926] VS_GUEST: print_page_table_entry: 66. VS_INFO.   PTE Value: 800010f933163
[22349.802142] VS_GUEST: judge: 10. VS_INFO. code page trampoline succeed.
[22349.803122] VS_GUEST: vs_driver_rsrv_init: 275. VS_INFO. -----Data Trampoline Part:-----
[22349.803124] VS_GUEST: print_page_table_entry: 34. VS_INFO. belonging of pagetable walk: insmod
[22349.804372] VS_GUEST: print_page_table_entry: 65. VS_INFO. Page table entry at virtual address ffff9defc2f8c000:
[22349.810330] VS_GUEST: print_page_table_entry: 66. VS_INFO.   PTE Value: 8008000102f8c163
[22349.812107] VS_GUEST: judge: 18. VS_INFO. data page trampoline succeed.
[22349.813172] VS_GUEST: print_page_table_entry: 34. VS_INFO. belonging of pagetable walk: insmod
[22349.813177] VS_GUEST: print_page_table_entry: 65. VS_INFO. Page table entry at virtual address ffff9defc2f8d000:
[22349.815937] VS_GUEST: print_page_table_entry: 66. VS_INFO.   PTE Value: 8008000102f8d163
[22349.817196] VS_GUEST: judge: 18. VS_INFO. data page trampoline succeed.
```
If you failed to see the info that all the code and data page trampoline succeed, please remove the module first, and try to re-install the module, until you've seen the succeed info.
```
rmmod svsm_guest.ko
insmod svsm_guest.ko
```
If you've tried several times and still unable to see the 'succeed' info, maybe this time you are a little unlucky, please reboot the Guest VM and try again.
```
shutdown now
sudo ./launch-qemu.sh -hda ae.qcow2 -sev-snp -svsm svsm.bin
```
### Step 4: Choose whether you want to run `redis` and `fio`
If you only want to run `redis` and `fio`, we need the support of the occlum in the docker image. Please jump to `Step 8`.

If you want to run all the benchmarks, we can directly run them in the Guest OS without entering the docker image. Please jump to `Step 5`.

### Step 5: Install LINUX-SGX-SDK in Guest OS
Please enter `components/[mode]` directory, choose the one directory named `linux-sgx`.
```
cd components/[mode]/linux-sgx
```
Compile and build `linux-sgx` using the following commands:
```
make clean && make sdk_install_pkg_no_mitigation
```
Then, you will find the binary installer in the `linux-sgx`
```
cd linux/installer/bin
```
You will find a file with the name like `sgx-xxxxxx,bin`, please run it.
```
./sgx-xxxxxxx.bin
```
The scripts will ask you whether to install the linux-sgx-sdk in the current directory. Please answer `no`, and entering `/opt/intel` if the script ask you where to install the SDK. That way, the file will be intalled in `/opt/intel/sgxsdk`.

To make it accessible, we should use the following command.
```
source /opt/intel/sgxsdk/environment
```

And now, the `linux-sgx-sdk` is installed successfully in Guest OS.
### Step 6: Run benchmarks except for `redis` and `fio`
We provide guidence in `benchmarks/README.md` to introduce the benchmarks we used, and how we plot the graphs.

Here we quickly go through this. Please give the `execution` priviledge to all of the scripts first.
```
find "." -name "*.sh" -exec chmod +x {} \;
``` 

#### Microbenchmarks without Occlum
If your `[mode]` is `nsgx`, you can run `micro_withoutocclum.sh`, which includes the following benchmarks.
- Leaf instructions of Intel SGX: which demostrates the performance of SGX leaf instructions. (Leaf)
- Edge Call: which measures the time to switch world between the APP and Enclave. (microbenchmarks/ecall_ocall)
- Lmbench: evaluate the memory bandwidth. (microbenchmarks/lmbench)
- sgx-nbench: CPU, FPU, memory system benchmarks. (microbenchmarks/sgx-nbench)
- Wolfssl: computation-intensive tasks.

To plot the graph later, results of the last three benchmarks should be kept in Files. 
```
./micro_withoutocclum.sh NSGX.txt
```
Which will automatically store the result of Lmbench, sgx-nbench, wolfssl in `nsgx.txt` in their own directory. 


If your `[mode]` is `sim`, please choose `micro_sim.sh` instead.

To compare the results with SIM mode of SGX, we provide another version of benchmarks, named `micro_sim.sh`.
```
./micro_sim.sh SIM.txt
```
#### Macrobenchmarks without Occlum
Serveral other macrobenchmarks can be obtained automatically through `macro_withoutocclum.sh`.
- hashjoin: another computation-intensive task.
- sqlite-native: SGX-version of SQlite.

Still we should record the performance in files. The `[mode]` here is the mode you choosed in `Step 2`. 
```
./macro_withoutocclum.sh [mode].txt
```
### Step 7: Benchmarks that need to be run manually
Another real application that we can run is the `tls`.

We still have some bug and limitation in the implementation of `NestedSGX`, the `tlserver` in `realworldapps/tls` is not able to launch smoothly. Even though we can get the handshake times as we expect, when all the threads exit in `pthread.join ` step, this app will crash. In that case, after recording the handshake times, we should kill the `qemu` process in host OS, and repeat the process to record the next handshake times.

```
ps aux | grep qemu          # Get the process id.
kill -9 <id_0> <id_1>       # Kill the invovled process.
```

We hope in the future we can aid this obstacle, though it is now sufficient to see the performance differences between `NestedSGX` and `SIM` mode of SGX.

Please refer to the `README.md` in tls if you want to have a look.

If you run in `sim` `[mode]` of `tlserver`, this isn't a problem.

### Step 8: Enter the docker image [Run benchmarks with the help of occlum]
The environment to run `redis` and `fio` is established in the docker image, with the support of LibOS named occlum.

So to run `redis` and `fio`, we should first enter the docker image, and prepare the corresponding environment to run benchmarks in our `[mode]` mode.
```
docker ps -a <image_id>         # Find the docker image ID.
docker start -ai <image_id>     # Start the docker image with <image_id> ID.

# In another terminal of the Occlum
docker exec -it /bin/bash       # enter the docker image in another terminal. Helpful if you are using tmux.
```
### Step 9: Set up LINUX-SGX-SDK in the docker image
Please enter `components/[mode]` directory, choose the one directory named `occlum-linux-sgx`. The `occlum-linux-sgx` is a little different from the one we use in the Guest OS, and is in corresponding to the `occlum` we use in the docker image. And the other differences are the toolchains in the docker image to build the SDK.
```
cd components/[mode]/occlum-linux-sgx
```
Compile and build `occlum-linux-sgx` using the following commands:
```
make clean && make sdk_install_pkg_no_mitigation
```
Then, you will find the binary installer in the `linux-sgx`
```
cd linux/installer/bin
```
You will find a file with the name like `sgx-xxxxxx,bin`, please run it.
```
./sgx-xxxxxxx.bin
```
The scripts will ask you whether to install the linux-sgx-sdk in the current directory. Please answer `no`, and entering `/opt/intel` if the script ask you where to install the SDK. That way, the file will be intalled in `/opt/intel/sgxsdk`. You don't have to worry if you do similar things in **Step 5** and install the former SDK in the same path before. Because the scripts will *automatically override* the one you use before and it will work all right just as our expectations.

To make it accessible, we should use the following command.
```
source /opt/intel/sgxsdk/environment
```
### Step 10: Set up Occlum in the docker image
Please enter `components/[mode]` directory, choose the one directory named `occlum`. Similar to what we describe in `set_occlum.sh`, running the following commands is sufficient.
```
make submodule && SGX_MODE=SIM make && make install
```
This will automatically install the new occlum in the environment. And the newer-installed occlum will automatically override the one we installed before.
### Step 11: Run `fio`
You can now run `micro_withocclum.sh` to run `fio`, like below: taking `[mode]` as `nsgx` for example.
```
/micro_withocclum.sh NSGX.txt
```
The result will be automatically stored in `fio`.

### Step 12: Run `redis`
Please refer to the `README.md` in `benchmarks/realworldapps/redis` directory for guidence.

### Step 13: Try `Hotcall`
`Hotcall` is also a directory in `components`, we change lots of the code in not only the `occlum` and `occlum-linux-sgx` to improve the performance of `fio` and `redis` of NestedSGX.

As we mentioned before, these two benchmarks are based on the docker image.

If you want to have a try, please change your `[mode]` into `hotcall` (i.e. select `components/Hotcall`), and repeat the `Step 9` and `Step 10` to install `occlum-linux-sgx` first, and then the `occlum`.

And run `Step 10` and `Step 11` again with `[mode]` set to `Hotcall`. (Yeah it is a little tedious....)

`Hotcall` use some Radical methods from *Hotcall@ISCA'17* to optimize, and sometimes it might fail. It is OK if you choose not to run `Hotcall` because it is not so stable, to be honest.
### Step 14: Plotting
If you've completed `nsgx` and `sim` benchmarks, you can now plot the graph.

We provide plotting benchmarks in each of the benchmark directory, part of the python code we provide might like this:
```python
parser.add_argument("--simulation", "-sim", type=str, help="Baseline result (*.res)")
parser.add_argument("--nestedsgx", "-nsgx", type=str, help="NestedSGX result (*.res)")
```
So you can plot and get the graph like this: the `SIM.txt` and `NSGX.txt` are obtained in the fore-mentioned steps.
```
python plot.py -sim SIM.txt -nsgx NSGX.txt
```
After plotting some graphs, you might need first to use `scp` to download the related `benchmarks` directory to the `Host OS` and have a look using some pdf extensions of `VSCODE` to have a look at the graphs.

## Some other Tips might be helpful: in the form of possible Q & A
### Q1: In the Guest OS, how can I shift my `[mode]` from `sim` to `nsgx`?
### A1: Please enter the `components/nsgx`, and reinstall `linux-sgx` like **Step 5**.

### Q2: In the docker Image, how can I shift my `[mode]` from `sim` to `nsgx`?
### A1: Please enter the `components/nsgx`, and reinstall `occlum-linux-sgx` like **Step 9** and `occlum` like **Step 10**.

### Q3: Can I run scripts like `micro_withoutocclum.sh` in the docker image instead of in the Guest OS?
### A3: Yes you can! But the performance results you get might be a little different since docker has some restrictions on the resources that the OS can utilize.

### Q4: This time, I accidently come across the `User App: IOCTL failed` problems, how can I mitigate this?
### A4: You might forget to install the Guest Kernel Module first. I recommend you first reboot the Guest OS, install the Kernel Module first, and then do the following things. BTW, if you come across this problem in the docker image, we are sorry that installing the Kernel Module after entering the Docker Image won't help aid this problem because the docker image is set up at the hypothesis that a `misc_device` named `/dev/vmpl-driver` is established before, which is set up by the Guest Kernel Module.


## Miscs:
Thank you for your great patience! We appreciate your supports a lot!