.section .text

#define REGS_UPDATE						 \
	movq	0(%r14), %rax						;\
	movq	8(%r14), %rbx						;\
	movq	16(%r14), %r13						;\
	movq	24(%r14), %rdx						;\
	movq	32(%r14), %rsi						;\
	movq	40(%r14), %rdi						;\
	movq    48(%r14), %rbp                      ;\
    movq    56(%r14), %rsp                      ;\
    movq    64(%r14), %rcx                      ;\
    movq    %r15, %r11                          

#define REQ_SENT						\
	movq 0(%r14), %rax							;\
	movq 8(%r14), %rcx							;\
	movq 16(%r14), %rdx							;\
	movq 24(%r14), %r8							;\
	movq 32(%r14), %r9							;\
	vmgexit										;\


#define REQ_RECE						\
	movq %rax, 0(%r14)							;\
	movq %rcx, 8(%r14)							;\
	movq %rdx, 16(%r14)							;\
	movq %r8, 24(%r14)							;\
	movq %r9, 32(%r14)							;\



.globl  back_trace
back_trace:
    
    REGS_UPDATE
    
    swapgs
    
    sysretq

# No need to change cr3, got the same virtual address space.
# it seems to be handy.



.globl vmgexit_process
vmgexit_process:
	REQ_SENT

	REQ_RECE

	ret