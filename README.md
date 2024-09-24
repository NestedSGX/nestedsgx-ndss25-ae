## NestedSGX
NestedSGX is a Framework utilizing AMD SEV-SNP features to simulate SGX with fair security ensurance and compatibility.

**For security**: We co-operate the threat model of SGX and SEV-SNP, the enclave distrusts the Guest OS and the Hypervisor.

**For compatibility**: Our NestedSGX can smoothly lanuch the LibOS based on SGX named [Occlum](https://github.com/occlum/occlum), therefore some large programs like `redis` and `lighttpd` can be easily ported to `SGX-like` form and successfully operate in a more secure environment.

NestedSGX is developed based on [`Linux-svsm`](https://github.com/AMDESE/linux-svsm), many thanks to the developers in AMD!

For more details, please refer to our paper.
```C
@article{wang2024nestedsgx,
  title={The Road to Trust: Building Enclaves within Confidential VMs},
  author={Wang, Wenhao and Song, Linke and Mei, Benshan and Liu, Shuang and Zhao, Shijun and Yan, Shoumeng and Wang, XiaoFeng and Meng, Dan and Hou, Rui},
  journal={arXiv preprint arXiv:2402.11438},
  year={2024}
}
```

### Preparation for NestedSGX:
Please build the [`Linux-svsm environment`](https://github.com/AMDESE/linux-svsm) first, to reproduce, your machine should support SEV-SNP features. After building the environment according to `linux-svsm`, what you have in current machine will be: (`vx` represents the newest svsm version of [linux branch](https://github.com/AMDESE/linux))
1. Host Machine: `svsm-preview-vx-host` version of linux.
2. Guest Machine: `svsm-preview-vx-guest` version of linux.
3. Qemu: corresponding version of the linux.
4. omvf: corresponding version of the linux.
5. svsm: original version of SVSM.

The above five components can be automatically downloaded and established by running `./build.sh --package` in `linux-svsm` project. Please refer to the initial project for more information and guidance. Besides, to run `Occlum` and `Intel SGX-SDK`, we prefer you to have at least `60 GB` space in your qemu hardware disk, or it might be not sufficient to support `Redis` and other `apps` based on `Occlum`.

NestedSGX is built based on `linux-svsm` project, below are some changes to make NestedSGX accessible. 

### Steps:
#### Use our Monitor (i.e. `svsm.bin`):** 

launch the qemu provided in `linux-svsm` project with our `svsm.bin`(instead of the initial version of `svsm.bin` provided in `linux-svsm` project), i.e. `-svsm [your_path_of_nestedsgx_svsm_bin]/svsm.bin`:
```Shell
./launch-qemu.sh -hda guest.qcow2 -sev-snp -svsm [your_path_of_nestedsgx_svsm_bin]/svsm.bin
```

#### Install Guest Module in the Guest VM: 
You can use commands like `scp` to transfer the source file into the Guest VM. Make sure you build the kernel module in the Guest VM, using `make` and `insmod svsm_guest.ko` to enable `NestedSGX` Driver.

However, simply `insmod svsm_guest.ko` might not always work. Further check is The Guest Module is very crucial in `NestedSGX` because it provides the trampoline page for the monitor, please check `README.md` in `svsm-guest-occlum` for more details.

#### linux-sgx-sdk: 
We used a modified version of `linux-sgx-sdk`. Based on the modified version of [sgx-sdk](https://github.com/occlum/linux-sgx.git) by Ant Group, we've also modified a bit to support `NestedSGX`.

If you are not so familiar to the installation of the `linux-sgx-sdk`, please refer to the original project for `linux-sgx`. Specifically, `NestedSGX` depends on the `SIM` mode of the `linux-sgx`.

When the above 3 steps done, you can run all of the SampleCode of SGX that can be run in `SIM` mode in NestedSGX. **But if you want to run `redis` and other interesting programs based on Occlum, please continue.**
#### Install Occlum in Guest OS: 
Occlum relys on the docker environment provided by Ant Group, **please first make sure you have successfuly insmod the Guest Module in last step,** then, set up the environment like following:
```python
# pull and run the image that support the occlum.
docker run -it --privileged -v /dev/svsm_guest:[your_rootpath_of_image_in_the_guestOS] occlum/occlum:0.29.7-ubuntu20.04

# set up the modified linux-sgx environment like step 3, make sure you've installed the sgxsdk in the path named '/opt/intel/sgxsdk/'
source /opt/intel/sgxsdk/environment
```
we use a modified `v0.29.7` version of Occlum, see following steps to override the default occlum in the docker. please use `scp` or other ways to copy occlum into your docker, then build the occlum:
```shell
make submodule && SGX_MODE=SIM make && make install
```
after that, you can run some programs like `fio` or `redis` based on Occlum. 


#### benchmarks: 
We use several benchmarks to measure the performance:
- microbenchmarks: as described in our paper. 4 kinds of Micro-benchmarks are involved.
- Real world applications: as described in our paper. 4 kinds of real apps are involved.

Please refer to the `benchmarks` for more details.

### HotCall: Acceleration
To accelerate the speed of Apps supported by Occlum, we provide another version of sdk based on [`HotCall(ISCA' 2017)`](https://github.com/oweisse/hot-calls.git), **this will be included in `hotcall`**. This can be in-cooperated with another version of the Occlum, which is also acclerated.

**Another version of the Occlum is also provided in combination with Hotcall version of `sgx-sdk`, make sure you have installed the Hotcall version of `sgx-sdk` before continue to build Hotcall version of Occlum. Then, the `fio` and `redis` can achieve better performance.**

This will be provided in [this repo](https://github.com/NestedSGX/Hotcall.git)