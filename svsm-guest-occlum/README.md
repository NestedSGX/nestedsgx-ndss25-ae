## Guest Module For NestedSGX.
To enable the ability of the NestedSGX, We should set up a guest module here.

To fulfill the need of user apps to use new features added by Guset OS and SVSM, besides the enclave the SVSM provides, it is irresistable to add a new module.

### What's the magic?
The role this module plays is to create an interface for apps in vmpl1 & ring 3 to use syscall to act on enclaves.

Draw a simple picture here is necessary.

|      \      |     ring 0  |    ring 3     |
| :----:      |    :----:   |    :----:     |
|  vmpl 1     |  Guest OS   |  Apps         |
|  vmpl 0     |   SEH       |  Enclave      |

The role this module acts is the Guest OS, it interacts with the SEH and Apps.

In SEH, we use `vc` provided in AMD SVSM-preview version of linux to run a virtual machine in VMPL1 configured by VMSA, this function is `vc_run_vmpl(VMPL::Vmpl1)`. Then, the SEH will blocked at `vmgexit` instruction utill the Guest OS kernel Module calls `vmgexit` and invokes the SEH.
### Design goals:
1. provide basic interface for user apps.
2. call `vmgexit` in a certain way to let SEH behave in a proper way.

### How can we add new features in Guest OS?
1. Use the specific Kernel version for Guest OS to compile.
2. Create a device for Apps to interface.

### Control Flow Details.
1. The user App first **open** the device.
2. Then it use **ioctl** to conduct some operations. In response, the device will convey the cmd arguments for the user apps. These parameters will be sent to the Hypervisor in VMSA data structure, and Hypervisor will transfer vmpl1 to vmpl0.
3. The Hypervisor responses and send parameters wrapped by VMSA back to the svsm.bin. SVSM can change the info of the VMSA of VMPL1.
4. The device receives the updated VMSA from the SEH.
5. The user App lastly **release** the device.

Besides, the Guest OS kernel Module can also interact with SEH without receiving request from Apps, when it comes to initialization and the exception handling of Enclave.

### Setup:
```
make && insmod svsm_guest.ko
```
The Kernel Module depends on the normal behavior of the Trampoline pages (code page and data page), but sometimes the Kernel Module might fail to successfully set up the trampoline. When you `insmod` the kernel module and get some log like below, that means the trampoline has been successfully set up. Therefore the Kernel Module can operate normally.
```
[   48.415430] VS_GUEST: print_page_table_entry: 32. VS_ERROR. belonging of pagetable walk: insmod
[   48.415882] VS_GUEST: print_page_table_entry: 63. VS_ERROR. Page table entry at virtual address ffff9683d7dd8000:
[   48.417179] VS_GUEST: print_page_table_entry: 64. VS_ERROR.   PTE Value: 8000117dd8163
[   48.417735] VS_GUEST: judge: 9. VS_ERROR. code page trampoline succeed.
[   48.418213] VS_GUEST: print_page_table_entry: 32. VS_ERROR. belonging of pagetable walk: insmod
[   48.418214] VS_GUEST: print_page_table_entry: 63. VS_ERROR. Page table entry at virtual address ffff9683d7dd9000:
[   48.419506] VS_GUEST: print_page_table_entry: 64. VS_ERROR.   PTE Value: 8000117dd9163
[   48.420072] VS_GUEST: judge: 9. VS_ERROR. code page trampoline succeed.
[   48.420532] VS_GUEST: vs_driver_rsrv_init: 251. VS_ERROR. -----Data Trampoline Part:-----
[   48.420533] VS_GUEST: print_page_table_entry: 32. VS_ERROR. belonging of pagetable walk: insmod
[   48.421096] VS_GUEST: print_page_table_entry: 63. VS_ERROR. Page table entry at virtual address ffff9683d0234000:
[   48.422399] VS_GUEST: print_page_table_entry: 64. VS_ERROR.   PTE Value: 8008000110234163
[   48.422957] VS_GUEST: judge: 16. VS_ERROR. data page trampoline succeed.
[   48.423420] VS_GUEST: print_page_table_entry: 32. VS_ERROR. belonging of pagetable walk: insmod
[   48.423421] VS_GUEST: print_page_table_entry: 63. VS_ERROR. Page table entry at virtual address ffff9683d0235000:
[   48.424750] VS_GUEST: print_page_table_entry: 64. VS_ERROR.   PTE Value: 8008000110235163
[   48.425309] VS_GUEST: judge: 16. VS_ERROR. data page trampoline succeed.
```
If you don't see the `succeed` log, you should `rmmod` the kernel module and re-insmod the kernel module until you see `succeed` log in data & code pages alike. Sometimes you might be unlucky and might have to `reboot` the guest OS and do the above things.

