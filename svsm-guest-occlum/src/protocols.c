#include "../headers/protocols.h"
#include "../headers/log.h"

extern void vmgexit_process(void);

// #pragma GCC optimize ("O0")

// Below 3 functions are included in sev.c, however we can't call them in any headers.
// So we simply add them here.
// -------------------------------------------------------
// when __rdmsr the MSR_SVSM_CAA, it will return the gpa physical address.
static inline u64 sev_snp_rd_caa_msr(void)
{
	return __rdmsr(MSR_SVSM_CAA);
}

// write to GHCB MSRs.
static __always_inline void sev_es_wr_ghcb_msr(u64 val)
{
	u32 low, high;

	low  = (u32)(val);
	high = (u32)(val >> 32);

	native_wrmsr(MSR_AMD64_SEV_ES_GHCB, low, high);
}

// read from GHCB MSRs.
static inline u64 sev_es_rd_ghcb_msr(void)
{
	return __rdmsr(MSR_AMD64_SEV_ES_GHCB);
}


/// @brief __svsm_msr_protocol: use 'caa' and 'request', vmmcall a msr protocol.
/// @param caa: calling address area.
/// @param request: svsm_service_request, parameters will be passed to vmsa.
/// @return success or fail.
static int /*__attribute__((optimize("O0")))*/ __svsm_msr_protocol(struct svsm_caa *caa, struct svsm_service_request *request)
{
	u64 val, resp;
	u8 pending;
	int ret;

	val = sev_es_rd_ghcb_msr();

	sev_es_wr_ghcb_msr(GHCB_MSR_VMPL_REQ_LEVEL(0));
	
	__asm__ __volatile__(
		"mov %0, %%r14\n\t"
		"movb $1, %1\n\t"
		:
		: "r" ((unsigned long) request), "m" (caa->call_pending)
		:
	);

	vmgexit_process();

	ret = request->rax;

	resp = sev_es_rd_ghcb_msr();

	sev_es_wr_ghcb_msr(val);

	pending = 0;
	asm volatile("xchgb %0, %1" : "+r" (pending) : "m" (caa->call_pending) : "memory");
	
	if (pending)
		ret = -EINVAL;

	if (GHCB_RESP_CODE(resp) != GHCB_MSR_VMPL_RESP)
		return -EINVAL;

	if (GHCB_MSR_VMPL_RESP_VAL(resp) != 0)
		return -EINVAL;

	return ret;
}


/// @brief svsm_service_handle_msr
/// @param request: the svsm_service_request
/// @return success or fail
static int svsm_service_handle_msr(struct svsm_service_request *request) {
    struct svsm_caa *new_caa;
    unsigned long flags;
    int ret;

    new_caa = this_cpu_read(svsm_caa);

    local_irq_save(flags);

    if (new_caa == NULL) {
        new_caa = (struct svsm_caa *) __va(sev_snp_rd_caa_msr());
        this_cpu_write(svsm_caa, new_caa);
    }

    ret = __svsm_msr_protocol(new_caa, request);
    
	if (ret < 0)
		panic("Guest OS: Can't remap the SVSM CAA page, ret= %d\n", ret);
	
    local_irq_restore(flags);

    return ret;
}

/// @brief wrapper of svsm_service_handle
int svsm_service(struct svsm_service_request *request) {
	return svsm_service_handle_msr(request);
}