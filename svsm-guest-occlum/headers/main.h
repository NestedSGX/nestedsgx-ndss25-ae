/* SPDX-License-Identifier: MIT */

#ifndef MAIN_H
#define MAIN_H

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/miscdevice.h>
#include <linux/mutex.h>
// Here we try to write to the device.

// Wrapper for SVSM Protocols.
#define __MIX__(hi, lo)                         (((unsigned long int) hi << 32) | (unsigned long int) lo)

#define SVSM_FUNCTION(protocol, callid)         __MIX__(protocol, callid)
#define SVSM_QUERY_PROTOCOL(protocol, version)  __MIX__(protocol, version)

// SVSM_FUNCTIONS:
// 1. core protocol. => protocol: 0.
// 2. sgx protocol. => protocol: 4.
#define CORE        0
#define SGX         4


// Linux generic TEE subsystem magic number defined in <linux/tee.h>
#define SVSM_SGX_IOC_MAGIC          0xa4

// Protocols sender.
#define RSRV_INIT                   0x0
#define TRAMPOLINE_INIT             0x10
#define TRMAPOLINE_INIT_APP         0x11
#define TRAMPOLINE_INIT_IDT         0x12



// sgx protocols.
#define ECREATE                     0x1
#define EADD                        0x2
#define EINIT                       0x3
#define EENTER                      0x4
#define EEXTEND                     0x5
#define EWB                         0x7
#define ERESUME                     0xb
#define EAUG                        0x22
// sgx interrupt handlers.
#define IRET                        0x20
#define EBLOCK                      0x23

// for debug and basic test.
#define TEST 0xa

#define PERF 0xff
// ioctl definition
// to be used in unlocked_ioctl function, our request should follow this mode.
// Besides: To let the Device read infomation from userspace and bring them into the kernel space, we should use _IOR here.
// However, as we want the Guest OS module to return something to our user apps, we might as well add Write features here.
// Therefore, we choose to use _IOWR as a result.
#define SVSM_IOC_SEND_REQUEST(x)                _IOWR(SVSM_SGX_IOC_MAGIC, x, struct svsm_service_request)

#define SVSM_CORE                               SVSM_IOC_SEND_REQUEST(CORE)

#define SVSM_ECREATE                            SVSM_IOC_SEND_REQUEST(ECREATE)
#define SVSM_EADD                               SVSM_IOC_SEND_REQUEST(EADD)
#define SVSM_EINIT                               SVSM_IOC_SEND_REQUEST(EINIT)
#define SVSM_EENTER                             SVSM_IOC_SEND_REQUEST(EENTER)
#define SVSM_EEXTEND                            SVSM_IOC_SEND_REQUEST(EEXTEND)
#define SVSM_TEST                               SVSM_IOC_SEND_REQUEST(TEST)
#define SVSM_ERESUME                            SVSM_IOC_SEND_REQUEST(ERESUME)
#define SVSM_EAUG                               SVSM_IOC_SEND_REQUEST(EAUG) 
#define SVSM_EBLOCK                             SVSM_IOC_SEND_REQUEST(EBLOCK)
#define SVSM_EWB                                SVSM_IOC_SEND_REQUEST(EWB)
#define SVSM_TRAMPOLINE_INIT_APP                SVSM_IOC_SEND_REQUEST(TRMAPOLINE_INIT_APP)

#define SVSM_PERF                               SVSM_IOC_SEND_REQUEST(PERF)
// set a name for DEVICE
#define DEVICE_NAME                             "vmpl_sgx_driver"

// if the device operation is successful, 
// return SUCCESS as a result.
#define SUCCESS 0

#define PF_HANDLER 1

#define VC_CPUID 2

#define PKRU 3

#endif