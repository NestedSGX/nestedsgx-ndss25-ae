#include "../headers/enclave.h"
#include "../headers/protocols.h"
#include "../headers/log.h"
#include "../headers/handlers.h"
#include "../headers/pgwalk.h"
#include "../headers/main.h"
#include "../headers/interrupts.h"

extern void* sgx_tmp_page;

extern void* cdpage;

extern int Ncdpage;

extern unsigned long sev_encrypt_bit;

extern void* dtpage;

extern int Ndtpage;

extern void back_trace(void);


extern typeof(exc_page_fault) *exc_page_fault_sym;

extern struct mutex ioctl_lock;

extern int iret;

unsigned long eenter_num = 0;
// grammar sugers
// ############################################
int Copy_from_user(void* dst, void* src, unsigned long size) {
    if (copy_from_user(dst, (int __user *) src, size)) {
        vs_err("Failed to copy_from_user\n");
        return -EFAULT;
    }
    return 0;
}

int Copy_to_user(void* dst, void* src, unsigned long size) {
    if (copy_to_user((int __user *) dst, src, size)) {
        vs_err("Failed to copy_to_user\n");
        return -EFAULT;
    }
    return 0;
}

/// @brief request_dump: simply dump the request
/// @param request: the svsm_service_request
void request_dump(struct svsm_service_request *request) {
    // vs_err("request info:\n rax: %llx rcx: %llx rdx: %llx r8: %llx r9: %llx\n", request->rax, request->rcx, request->rdx, request->r8, request->r9);
}


/// @brief handle_core: basic function that we used for core protocols and sgx protocols
/// the request is sent by the user apps, not the kernel module like rsrv and trampoline functions.
/// @param arg: user input arguments.
/// The detailed info about how to set arg will be concluded below
/// @return SUCCESS or -EFAULT
int handle_core(void __user *arg) {
    struct svsm_service_request request;
    // copy request from user, store it in svsm_service_request request.
    /*
    if (copy_from_user(&request, (int __user *) arg, sizeof(struct svsm_service_request))) {
        return -EFAULT;
    }
    */
    Copy_from_user(&request, (void*) arg, sizeof(struct svsm_service_request));
    // we use svsm_service_handle to handle the requeset.
    // return value will be sent back to userspace.
    // request_dump(&request);
    svsm_service(&request);
    // request_dump(&request);
    // copy result of request from kernel space to user space.
    Copy_to_user((void*) arg, &request, sizeof(struct svsm_service_request));
    /*
    if (copy_to_user((int __user *) arg, &request, sizeof(struct svsm_service_request))) {
        vs_warn("fail to regain the arg\n");
        return -EFAULT;
    }
    */
    return SUCCESS;
}

/// @brief handle_backtrace:
/// only for backtrace to return back to eexit function of VMPL1 Ring3.
/// @param arg: user input arguments.
/// The detailed info about how to set arg will be concluded below
/// @return SUCCESS or -EFAULT
int handle_backtrace(void __user *arg) {
    int ret = 0;
    unsigned long long cr2 = 0;
    // sp of interrupts.
    unsigned long long ir_sp = 0;
    struct pt_regs ctxt;
    struct svsm_service_request request;
    // copy request from user, store it in svsm_service_request request.
    /*
    if (copy_from_user(&request, (int __user *) arg, sizeof(struct svsm_service_request))) {
        return -EFAULT;
    }
    */
    Copy_from_user(&request, (void*) arg, sizeof(struct svsm_service_request));
    // we use svsm_service_handle to handle the requeset.
    // return value will be sent back to userspace.
    // request_dump(&request);
    // mutex_lock(&my_mutex);
    /*
    vs_err("[EENTER Mutex] get the mutex.");
    vs_err("virt address: %llx", request.rcx);
    */
    ret = svsm_service(&request);

    // in PF_HANDLER case.
    if (ret == PF_HANDLER) {
        mutex_lock(&ioctl_lock);
        iret = 1;
        while (ret) {
            memcpy(&ctxt, sgx_tmp_page, sizeof(ctxt));
            cr2 = request.rcx;
            ir_sp = request.rdx;
            __asm__ __volatile__(
                "mov %0, %%cr2\n\t"
                :
                : "a"(cr2)
                :
            );
            // handle the page fault.
            exc_page_fault_sym(&ctxt, ctxt.orig_ax);
            request.rax = SVSM_FUNCTION(SGX, IRET);
            request.rcx = ir_sp;
            request.rdx = __pa(current->mm->pgd);
            request.r8 = 0;
            request.r9 = 0;
            // set a while loop until we got a normal return value from the SVSM.
            ret = svsm_service(&request);
        }
        // after returning 0 here, EENTER 
        mutex_unlock(&ioctl_lock);
        // while(1);
        // return SUCCESS;
    } else if (ret == VC_CPUID) {
        mutex_lock(&ioctl_lock);
        iret = 1;
        while (ret) {
            memcpy(&ctxt, sgx_tmp_page, sizeof(ctxt));
            ir_sp = request.rcx;
            // EAX, EBX, ECX, and EDX 
            unsigned long long rax = ctxt.ax;
            unsigned long long rbx = 0;
            unsigned long long rcx = 0;
            unsigned long long rdx = 0;
            __asm__ __volatile__(
                "cpuid\n\t"
                : "=a"(rax), "=b"(rbx), "=c"(rcx), "=D"(rdx)
                : "a"(rax)
                :
            );
            struct pt_regs* result = (struct pt_regs *) ir_sp;
            result->ax = rax;
            result->bx = rbx;
            result->cx = rcx;
            result->dx = rdx;
            // the cpuid instruction has been executed. add 2 to jump over it.
            result->ip += 2;
            request.rax = SVSM_FUNCTION(SGX, IRET);
            request.rcx = ir_sp;
            request.rdx = __pa(current->mm->pgd);
            request.r8 = 0;
            request.r9 = 0;
            ret = svsm_service(&request);
        }
        mutex_unlock(&ioctl_lock);
    } 
    /*
    else if (ret == PKRU) {
        unsigned long start = request.rcx;
        unsigned long len = request.rdx;
        unsigned long prot = request.r8;
        unsigned long key = request.r9;
        if (len < PAGE_SIZE)
            set_pte_pkru(address, key);
        
    }
    */
    // now it's time to jump back to VMPL1 Ring3

    /* ********************* */
    // Field of request:
    // rcx: register address of the user app.
    // rdx: rflags of VMPL1 Ring3, provided by VMPL0 Ring3.
    // r8: fsbase of the user app in sgx sdk.
    // r9: gsbase of the user app in sgx sdk.
    // -------------------------------------------
    // Different from newer version of linux-sgx sdk,
    // We should set fs_base and gs_base by ourself before entering the user app.
    /* ********************* */
    unsigned long long fs_base = request.r8;
    unsigned long long gs_base = request.r9;
    /*
    vs_err("fsbase: %llx, gs_base: %llx", fs_base, gs_base);
    */
    // unsigned long gs_base = request.r9;
    __asm__ __volatile__(
        "wrfsbase %0\n\t"
        :
        : "a"(fs_base)
        :
    );
    

    // set the kernel gs base value back. => swapgs will let the user app to have gs value equal to 0.
    unsigned int eax = (unsigned int) gs_base;
    unsigned int edx = (unsigned int) (gs_base >> 32);
    __asm__ __volatile__(
        "wrmsr\n\t"
        :
        : "a"(eax), "d"(edx), "c"(0xc0000102)
        :
    );
    // vs_err("fsbase: %lx", fs_base);
    enclu_regs_t regs = {0};
    Copy_from_user(&regs, (void*) (request.rcx), sizeof(enclu_regs_t));
    /*
    if (copy_from_user(&regs, (int __user *) (request.rcx), sizeof(enclu_regs_t))) {
        return -EFAULT;
    }
    */
    /*
    vs_err("Dump register info in the kernel module in VMPL1 Ring0");
    vs_err("xax: %lx\n", regs.xax);
    vs_err("xbx: %lx\n", regs.xbx);
    vs_err("xcx: %lx\n", regs.xcx);
    vs_err("xdx: %lx\n", regs.xdx);
    vs_err("xsi: %lx\n", regs.xsi);
    vs_err("xdi: %lx\n", regs.xdi);
    vs_err("xbp: %lx\n", regs.xbp);
    vs_err("xsp: %lx\n", regs.xsp);
    vs_err("xip: %lx\n", regs.xip);
    */
    // mutex_unlock(&my_mutex);
    __asm__ __volatile__(
        "mov %0, %%r14\n\t"
        "mov %1, %%r15\n\t"
        :
        : "r" ((unsigned long) &regs), "r" (request.rdx)
        : 
    );
    back_trace();
    // never reach this place. 
    /*
    vs_err("You can't see me.");
    while (1) {

    }
    */
    // printk(KERN_INFO "Never see me.");
    return SUCCESS;
}

/// @brief handle_ecreate: use SECS to create an enclave
/// @param arg: SECS address passed from user
/// @return SUCCESS or -EFAULT
int handle_ecreate(void __user *arg) {
    struct svsm_service_request request;

    // copy request from user, store it in svsm_service_request request.
    Copy_from_user(&request, (void*) arg, sizeof(struct svsm_service_request));

    // the virtual address of secs in user app.
    unsigned long secs_va = request.rcx;
    
    // copy the secs info into the sgx_tmp_page,
    // the Monitor share the sgx_tmp_page with the Guest Module by using the same physical address.
    Copy_from_user(sgx_tmp_page, (void*) secs_va, PAGE_SIZE);

    svsm_service(&request);
    
    // clear the sgx_tmp_page.
    memset(sgx_tmp_page, 0, PAGE_SIZE);
    return SUCCESS;
}

/// @brief handle_eadd:  
/// @param arg 
/// @return 
int handle_eadd(void __user *arg) {
    struct svsm_service_request request;

    // copy request from user, store it in svsm_service_request request.
    Copy_from_user(&request, (void*) arg, sizeof(struct svsm_service_request));

    // the virtual address of secs in user app.
    unsigned long page_va = request.rcx;
    
    // copy the secs info into the sgx_tmp_page,
    // the Monitor share the sgx_tmp_page with the Guest Module by using the same physical address.
    Copy_from_user(sgx_tmp_page, (void*) page_va, PAGE_SIZE);

    svsm_service(&request);
    
    // clear the sgx_tmp_page.
    memset(sgx_tmp_page, 0, PAGE_SIZE);
    return SUCCESS;
}

int handle_eldb(void __user * arg) {
    struct svsm_service_request request;

    // copy request from user, store it in svsm_service_request request.
    Copy_from_user(&request, (void*) arg, sizeof(struct svsm_service_request));

    // the virtual address of secs in user app.
    unsigned long page_va = request.rcx;
    
    // copy the secs info into the sgx_tmp_page,
    // the Monitor share the sgx_tmp_page with the Guest Module by using the same physical address.
    Copy_from_user(sgx_tmp_page, (void*) page_va, PAGE_SIZE);

    svsm_service(&request);
    
    // clear the sgx_tmp_page.
    memset(sgx_tmp_page, 0, PAGE_SIZE);
    return SUCCESS;
}
/// @brief handle_eenter: use arg to enter the enclave
/// @param arg: many args
/// @return SUCCESS or -EFAULT
int handle_eenter(void __user *arg) {
    eenter_num += 1;
    struct svsm_service_request request;
    if (iret == 1) {
        mutex_lock(&ioctl_lock);
        iret = 0;
        mutex_unlock(&ioctl_lock);
    }
    // copy request from user, store it in svsm_service_request request.
    Copy_from_user(&request, (void*) arg, sizeof(struct svsm_service_request));
    // vs_err("virt address: %llx [EENTER] TCS address: %llx", request.rcx, request.rdx);
    request_dump(&request);
    // request_dump(&request);
    /* ------------------------------------ */    
    // parameters we should send:
    // 1. rcx: the regs virtual address in VMPL1 Ring3.
    // 2. rdx: cr3 of user.
    // 3. r8: fs base of the user address.
    /* ------------------------------------ */    

    request.rdx = __pa(current->mm->pgd);
    Copy_to_user((void*) arg, &request, sizeof(struct svsm_service_request));

    handle_backtrace(arg);


    return SUCCESS;
}



int handle_test(void __user *arg) {

    return SUCCESS;
}

// in handle_tramp request from the user app, we simply pgwalk for the trampoline code pages and the trampoline idt data page
int handle_tramp(void __user *arg) {
    for (int i = 0; i < Ncdpage; i++) {
        app_pgwalk((unsigned long) __va(__pa(cdpage) + PAGE_SIZE * i), (unsigned long) (__pa(cdpage) + PAGE_SIZE * i + sev_encrypt_bit), 0);
    }

    for (int i = 0; i < Ndtpage; i++) {
        app_pgwalk((unsigned long) __va(__pa(dtpage) + PAGE_SIZE * i), (unsigned long) (__pa(dtpage) + PAGE_SIZE * i + sev_encrypt_bit), 1);
    }
    /*
    vs_err("----------- data trampoline in User App -------------------");
    for (int i = 0; i < Ndtpage; i++) {
        print_page_table_entry((unsigned long) __va(__pa(dtpage) + PAGE_SIZE * i), 1);
    }

    vs_err("tss: %llx", __va(__pa(dtpage) + PAGE_SIZE));
    */
    /*
    for (int i = 0; i < 256; i++) {
        unsigned char* c = (unsigned char*) (__va(__pa(dtpage) + PAGE_SIZE + i));
        vs_err("%d: %02x", i, (*c));
    }
    */
    handle_core(arg);
    return SUCCESS;
}

int handle_eresume(void __user *arg) {
    handle_core(arg);
    return SUCCESS;
}

int handle_eaug(void __user *arg) {
    handle_core(arg);
    return SUCCESS;
}

int handle_eblock(void __user *arg) {
    handle_core(arg);
    return SUCCESS;
}

int handle_emodpr(void __user * arg) {
    handle_core(arg);
    return SUCCESS;
}

int handle_einit(void __user *arg) {
    struct svsm_service_request request;

    // copy request from user, store it in svsm_service_request request.
    Copy_from_user(&request, (void*) arg, sizeof(struct svsm_service_request));

    // the virtual address of init_descriptor in user app.
    unsigned long init_desc = request.rcx;
    
    // copy the secs info into the sgx_tmp_page,
    // the Monitor share the sgx_tmp_page with the Guest Module by using the same physical address.
    Copy_from_user(sgx_tmp_page, (void*) init_desc, sizeof(uint64_t) * 2);

    svsm_service(&request);
    
    // clear the sgx_tmp_page.
    memset(sgx_tmp_page, 0, sizeof(uint64_t) * 2);
    return SUCCESS;
}

int handle_eextend(void __user *arg) {
    handle_core(arg);
    return SUCCESS;
}

int handle_ewb(void __user *arg) {
    struct svsm_service_request request;

    // copy request from user, store it in svsm_service_request request.
    Copy_from_user(&request, (void*) arg, sizeof(struct svsm_service_request));

    svsm_service(&request);

    // copy the secs info into the sgx_tmp_page,
    // the Monitor share the sgx_tmp_page with the Guest Module by using the same physical address.
    Copy_to_user((void *)(request.rcx), sgx_tmp_page, PAGE_SIZE);

    // clear the sgx_tmp_page.
    memset(sgx_tmp_page, 0, PAGE_SIZE);

    return SUCCESS;
}

int handle_perf(void __user *arg) {
    vs_err("eenter_num: %llx\n", eenter_num);
    return SUCCESS;
}

// For benchmarking simplicity, we combine eremove and ecreate as a whole.
// In fact we kinda little use the actual eremove instruction during our process.
int handle_eremove(void __user *arg){
    struct svsm_service_request request;

    // copy request from user, store it in svsm_service_request request.
    Copy_from_user(&request, (void*) arg, sizeof(struct svsm_service_request));

    // the virtual address of secs in user app.
    unsigned long secs_va = request.rcx;
    
    // copy the secs info into the sgx_tmp_page,
    // the Monitor share the sgx_tmp_page with the Guest Module by using the same physical address.
    // Copy_from_user(sgx_tmp_page, (void*) secs_va, PAGE_SIZE);

    svsm_service(&request);
    
    // clear the sgx_tmp_page.
    memset(sgx_tmp_page, 0, PAGE_SIZE);
    return SUCCESS;
}