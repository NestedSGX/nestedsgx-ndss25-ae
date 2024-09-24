#ifndef ENCLAVE_H
#define ENCLAVE_H

#include <linux/types.h>
#include <asm/current.h>
#include <asm/pgtable.h>

typedef struct _enclu_regs_t
{
    uintptr_t xax;
    uintptr_t xbx;
    uintptr_t xcx;
    uintptr_t xdx;
    uintptr_t xsi;
    uintptr_t xdi;
    uintptr_t xbp;
    uintptr_t xsp;
    uintptr_t xip;
} enclu_regs_t;

#endif