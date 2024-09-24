#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <linux/percpu-defs.h>
#include <asm/uaccess.h>
#include <asm/sev.h>
#include <asm/msr-index.h> // MSR_SVSM_CAA register is defined here.
#include <asm/svm.h>

// Here we try to write to the device.

// From SVSM draft specification:
// Core protocol only change these 5 registers.
// As for generality, other protocols here should also use 5 registers.
struct svsm_service_request {
    unsigned long long rax;
    unsigned long long rcx;
    unsigned long long rdx;
    unsigned long long r8;
    unsigned long long r9;
};

// Protocol type.
static DEFINE_PER_CPU(struct svsm_caa *, svsm_caa) = NULL;
static DEFINE_PER_CPU(struct ghcb *, ghcb) = NULL;

int svsm_service(struct svsm_service_request *request);
#endif 