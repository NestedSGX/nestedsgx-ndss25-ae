#include "../headers/handlers.h"
#include "../headers/protocols.h"
#include "../headers/pgwalk.h"
#include "../headers/utils.h"
#include "../headers/main.h"
#include "../headers/interrupts.h"

// how many device is open now.
static int device_open_count = 0;

void* sgx_tmp_page = 0;

void* cdpage = 0;

int Ncdpage = 0;

int Ndtpage = 0;

void* dtpage = 0;

unsigned long sev_encrypt_bit = 0;

int iret = 0;

DEFINE_MUTEX(ioctl_lock);

// inner function from the linux kernel.
typeof(__p4d_alloc) *__p4d_alloc_sym;
typeof(__pud_alloc) *__pud_alloc_sym;
typeof(__pmd_alloc) *__pmd_alloc_sym;
typeof(__pte_alloc) *__pte_alloc_sym;
typeof(exc_page_fault) *exc_page_fault_sym;

// when syscall open is operated on this device,
// It will automatically run this function.
static int vs_driver_open(struct inode *inode, struct file *file) {

    device_open_count++;
    // avoid being removed by others.
    try_module_get(THIS_MODULE);
    // vs_err("Now the device_open_count is: %d", device_open_count);
    // vs_err("Open Device.\n");
    return SUCCESS;
}

// In accordance with svsm_open, so basically do the reverse steps of svsm_open.
static int vs_driver_release(struct inode *inode, struct file *file)
{
    device_open_count--;
    module_put(THIS_MODULE);
    // vs_err("Release Device.\n");
    return SUCCESS;
}

// Judge order in __get_free_page function.
static int fetch_order(int order, int order_cursor) 
{
    if (order_cursor < 0)
        return 0;
    while (order_cursor != 0) {
        order_cursor >>= 1;
        order += 1;
    }
    return order;
}

static int fetch_order_data(int order_cursor) {
    if (order_cursor < 5) {
        return 1;
    } else if (order_cursor >= 5 && order_cursor < 13) {
        return 2;
    } else if (order_cursor >= 13 && order_cursor < 29) {
        return 3;
    }
    vs_err("Too many cores, I bet we can't support this.");
    return 0;
}

// Main function that we handle the request of the user apps.
// Apps should follow standards we set here to interact with the device.
static long vs_driver_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{   
    long ret = 0;
    switch (cmd) {
        case SVSM_CORE:
            //vs_err("SVSM CORE\n");
            ret = handle_core((void __user *) arg);
            break;
        case SVSM_ECREATE:
            // vs_err("SVSM_ECREATE\n");
            ret = handle_ecreate((void __user *) arg);
            break;
        case SVSM_EADD:
            //vs_err("SVSM_EADD\n");
            ret = handle_eadd((void __user *) arg);
            break;
        case SVSM_ELDB:
            ret = handle_eldb((void __user *) arg);
            break;
        case SVSM_EENTER:
            // vs_err("SVSM_EENTER\n");
            ret = handle_eenter((void __user *) arg);
            break;
        case SVSM_EINIT:
            ret = handle_einit((void __user *) arg);
            break;
        case SVSM_EREMOVE:
            ret = handle_eremove((void __user *) arg);
            break;
        case SVSM_EEXTEND:
            // vs_err("SVSM_FSGS\n");
            ret = handle_eextend((void __user *) arg);
            break;
        case SVSM_EWB:
            ret = handle_ewb((void __user *) arg);
            break;
        case SVSM_TEST:
            //vs_err("SVSM_TEST\n");
            ret = handle_test((void __user *) arg);
            break;
        case SVSM_TRAMPOLINE_INIT_APP:
            // vs_err("SVSM_TRAMPOLINE_INIT_APP");
            ret = handle_tramp((void __user *) arg);
            break;
        case SVSM_ERESUME:
            ret = handle_eresume((void __user *) arg);
            break;
        case SVSM_EAUG:
            ret = handle_eaug((void __user *) arg);
            break;
        case SVSM_EBLOCK:
            ret = handle_eblock((void __user *) arg);
            break;
        case SVSM_EMODPR:
            ret = handle_emodpr((void __user *) arg);
            break;
        case SVSM_PERF:
            ret = handle_perf((void __user *) arg);
            break;
        default:
            //vs_err("default\n");
            return -EFAULT;
    }
    return ret;
}

// file operation of the svsm_guest_device.
static const struct file_operations vs_driver_misc_fops = {
    .owner = THIS_MODULE,
    .open = vs_driver_open,
    .release = vs_driver_release,
    .unlocked_ioctl = vs_driver_unlocked_ioctl,
};

// Prepare to register the svsm_misc_device.
static struct miscdevice vs_driver_misc_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &vs_driver_misc_fops,
    .mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH,
};

/// @brief vs_driver_rsrv_init: send request to the SVSM for initialization of the shared memory.
/// at the same time, receive trampoline info from the Rust-Monitor.
/// 
/// Two kinds of memory will be shared between the Rust-Monitor and the Linux Kernel:
/// 1. a page for passing SGX related info.
/// 2. trampoline pages. 
///     i. code trampoline pages.
///    ii. data trampoline page.
static int vs_driver_rsrv_init(void) {
    // a page for passing SGX related info.
    sgx_tmp_page = get_zeroed_page(GFP_KERNEL);
    vs_err("sgx_tmp_page: %lx", sgx_tmp_page);
    
    struct svsm_service_request request = {
        .rax = SVSM_FUNCTION(SGX, RSRV_INIT),
        .rcx = __pa(sgx_tmp_page),
    };

    svsm_service(&request);
    vs_err("first request handled.");
    vs_err("Ncdpage: %lx", request.r8);
    int Ncdpage = request.r8;

    // get Ncdpage in reality.
    int cd_order = fetch_order(-1, Ncdpage);
    cdpage = __get_free_pages(GFP_KERNEL, cd_order);
    Ncdpage = 1;
    for (int i = 0; i < cd_order; i++) {
        Ncdpage <<= 1;
    }
    vs_err("cdpage: %lx", cdpage);
    vs_err("Ncdpage: %d", Ncdpage);
    
    for (int i = 0; i < Ncdpage; i++) {
        vs_err("cdpage[%d]: virt address: %lx phys address: %lx", i, __va(__pa(cdpage) + PAGE_SIZE * i), __pa(cdpage) + PAGE_SIZE * i);
    }
   
    // allocate the tramp_data_page. => AEX handling or for debugging.
    // in tramp_data_page, we should allocate the GDT, TSS in data trampoline, and ist stack, cpl0 stack for each core.
    
    // Basic usage graph of data page of trampoline.
    /*
    
     0    128   256         640          1024                           2048 ...............
     ------------------------------------------------------------------------------------
     | GDT | TSS |    ist    |     cpl0     | (blank) | TSS | ist | cpl0 | ........ | ................
     ------------------------------------------------------------------------------------
        \                    /                \                        /
            -- core 0 --                             -- core 1 --
      
     */
    
    // first get to know how many cpu cores we are using.
    int cpu_num = num_possible_cpus();
    vs_err("cpu_num: %d", cpu_num);
    
    // In common case, we support core number below or equal to 7.
    // For NUMA cases, the allocation might fail.
    // To Judge whether the NUMA cases are supported or not, please pay attention to the 'page info' we get in the kernel module.

    int dt_order = fetch_order_data(cpu_num);
    dtpage = __get_free_pages(GFP_KERNEL, dt_order);
    Ndtpage = 1;
    for (int i = 0; i < dt_order; i++) {
        Ndtpage <<= 1;
    }
    vs_err("dtpage: %lx", dtpage);
    vs_err("Ndtpage: %lx", Ndtpage);

    // parameters we should send:
    // 1. pa of tramp_page in VMPL1.
    // 2. va of tramp_page in VMPL1.
    // 3. page num of trampoline.
    
    struct svsm_service_request request_trampoline = {
        .rax = SVSM_FUNCTION(SGX, TRAMPOLINE_INIT),
        .rcx = __pa(cdpage),
        .rdx = cdpage,
        .r8 = Ncdpage,
    };

    svsm_service(&request_trampoline);
    
    // for return value: r9 will return the sev_encrypt_bit.
    sev_encrypt_bit = request_trampoline.r9;

    /* ------------------------------------ */    
    // parameters we should send:
    // 1. pa of tramp_data_page in VMPL1.
    // 2. va of tramp_data_page in VMPL1.
    /* ------------------------------------ */    
    struct svsm_service_request request_data_trampoline = {
        .rax = SVSM_FUNCTION(SGX, TRAMPOLINE_INIT_IDT),
        .rcx = __pa(dtpage),
        .rdx = dtpage,
        .r8 = Ndtpage,
    };

    svsm_service(&request_data_trampoline);

    __asm__ __volatile__ (".byte 0x0f,0x01,0xfe" : : "a"(0xf), "c"(0), "d"(0) : );
    __asm__ __volatile__ (".byte 0x0f,0x01,0xff" : : : );
    vs_err("tlb flush is done");

    vs_err("-----Code Trampoline Part:-----");
    for (int i = 0; i < Ncdpage; i++) {
        set_pte_x((unsigned long) __va(__pa(cdpage) + PAGE_SIZE * i));
    }
    // data trampoline for idt don't need to set executable bit.
    for (int i = 0; i < Ncdpage; i++) {
        print_page_table_entry((unsigned long) __va(__pa(cdpage) + PAGE_SIZE * i), 0);
    }
    vs_err("-----Data Trampoline Part:-----");
    for (int i = 0; i < Ndtpage; i++) {
        print_page_table_entry((unsigned long) __va(__pa(dtpage) + PAGE_SIZE * i), 1);
    }
    return SUCCESS;
}

// Frame work of SVSM_MODULE for Guest OS.
// misc_register.
static int __init vs_driver_init(void) {
    int ret;

    vs_err("Device Module Hello!");
    
    // export inner function in Linux kernel.
    find_kallsyms_lookup_name();

    RESOLVE_EXTERNAL_SYMBOL(__p4d_alloc);
    RESOLVE_EXTERNAL_SYMBOL(__pud_alloc);
    RESOLVE_EXTERNAL_SYMBOL(__pmd_alloc);
    RESOLVE_EXTERNAL_SYMBOL(__pte_alloc);
    
    RESOLVE_EXTERNAL_SYMBOL(exc_page_fault);

    vs_err("kallsyms can be looked up now...");
    vs_err("Can you see me in the console?");
    if (vs_driver_rsrv_init() != 0) {
        pr_err("vs_driver_rsrv_init error\n");
        return -1;
    }
    
    // register the device.
    ret = misc_register(&vs_driver_misc_device);
    if (ret) {
        vs_warn("Failed to register misc device\n");
        return ret;
    }

    return 0;
}

// misc_deregister.
static void __exit vs_driver_exit(void) {
    misc_deregister(&vs_driver_misc_device);
    vs_err("Device Module Goodbye!\n");
}

module_init(vs_driver_init);
module_exit(vs_driver_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("VMPL SGX DRVIER for User Apps to Interact with.");
