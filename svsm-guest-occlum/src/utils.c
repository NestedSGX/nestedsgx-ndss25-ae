#include "../headers/utils.h"



// asistence function
int noop_pre(struct kprobe *p, struct pt_regs *regs) { 
    return 0; 
}

static struct kprobe kp = {   
	.symbol_name = "kallsyms_lookup_name",  
};
 
unsigned long (*kallsyms_lookup_name_fun)(const char *name) = NULL;


// find the address of the kallsyms_lookup_name_addr, and store this
int find_kallsyms_lookup_name(void) { 
	int ret = -1;
	kp.pre_handler = noop_pre;
	ret = register_kprobe(&kp);
    if (ret < 0) {  
	    vs_err("register_kprobe failed, error:%d\n", ret); 
        return ret; 
	}
	vs_err("kallsyms_lookup_name addr: %p\n", kp.addr); 
	kallsyms_lookup_name_fun = (void*)kp.addr; 
	unregister_kprobe(&kp);
	return ret;
}
