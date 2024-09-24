## NestedSGX
NestedSGX is a Framework utilizing AMD SEV-SNP features to simulate SGX with great security ensurance and compatibility.

**For security**: In our threat model, we not only take the HV in Host Machine as the attacker, but also take the Guest OS as the attacker, so we built our system with dual Isolation for the Enclave based on AMD VMPL features, which makes SGX programs running in NestedSGX harder to be attacked.

**For compatibility**: Our NestedSGX can support not only `SampleCodes` provided in `linux sgx-sdk`, but also can smoothly lanuch the LibOS based on SGX named [Occlum](https://github.com/occlum/occlum), therefore some large programs like `redis` and `lighttpd` can be easily ported to `SGX-like` form and successfully operate in a more security environment.

NestedSGX is based on [`Linux-svsm`](https://github.com/AMDESE/linux-svsm), many thanks to the developers in AMD!!

For more details, please refer to our paper.
### Usage and Installation:
Please build the [`Linux-svsm environment`](https://github.com/AMDESE/linux-svsm) first, to reproduce, your machine should support SEV-SNP features. After building the environment according to linux-svsm, what you have in current machine will be: (`x` represents the newest svsm version of [linux branch](https://github.com/AMDESE/linux))
1. Host Machine: svsm-preview-vx-host version of linux.
2. Guest Machine: svsm-preview-vx-guest version of linux.
3. Qemu: corresponding version of the linux.
4. omvf: corresponding version of the linux.
5. svsm

The above five components can be downloaded by running `./build.sh --package` in `linux-svsm` project, and can be set up according to the `README.md` of the same project. Please refer to the initial project for more information and guidance. Besides, to run `Occlum` and `Intel SGX-SDK`, we prefer you to have at least `60 GB` space in your qemu hardware disk.

NestedSGX is built based on `linux-svsm` project, below are some changes to make NestedSGX run. 
1. SEH: launch the qemu provided in `linux-svsm` project with our `svsm.bin`(replace the initial version of `svsm.bin` provided in `linux-svsm` project with our NestedSGX `svsm.bin`), i.e. `-svsm [your_path_of_nestedsgx_svsm_bin]/svsm.bin`:
```
./launch-qemu.sh -hda guest.qcow2 -sev-snp -svsm [your_path_of_nestedsgx_svsm_bin]/svsm.bin
```
2. Guest Module in the Guest VM: `make` and `insmod svsm_guest.ko` to enable NestedSGX Driver.
3. linux-sgx: we used a modified version of `linux-sgx-sdk`. To accelerate the speed of Apps supported by Occlum, we provide another version of sdk based on HotCall(ISCA' 2017), **this will be included in `hotcall`**.

When the above 3 steps done, you can run all of the SampleCode of SGX that can be run in SIM mode in NestedSGX. But if you want to run `redis` and other interesting programs based on Occlum, please continue.

4. occlum: occlum relys on the docker environment provided by Ant Group, please first make sure you have successfuly insmod the Guest Module, then, set up the environment like following:
```
# pull and run the image that support the occlum.
docker run -it --privileged -v /dev/svsm_guest:[your_rootpath_of_image_in_the_guestOS] occlum/occlum:0.29.7-ubuntu20.04
# set up the modified linux-sgx environment like step 3, make sure you install the sgxsdk in the path named '/opt/intel/sgxsdk/'
source /opt/intel/sgxsdk/environment
```
we use a modified `v0.29.7` version of Occlum, see following steps to get the occlum modified. please use `scp` all other ways to copy occlum into your docker, then build the occlum:
```
make submodule && SGX_MODE=SIM make && make install
```
after that, you can run some programs like `fio` or `redis` based on Occlum. 

**Another version of the Occlum is also provided in combination with Hotcall version of `sgx-sdk`, make sure you have installed the Hotcall version of `sgx-sdk` before continue to build Hotcall version of Occlum. Then, the `fio` and `redis` can achieve better performance.**

5. benchmarks: we use several benchmarks to measure the performance, two kinds of performance are shown below:
- microbenchmarks: as described in our paper. 4 kinds of Micro-benchmarks are involved.
- Real world applications: as described in our paper. 4 kinds of real apps are involved.