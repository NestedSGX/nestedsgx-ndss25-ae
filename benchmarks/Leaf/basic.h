/* SPDX-License-Identifier: MIT */
/*
 * Copyright (C) 2024 IIE of CAS.
 *
 * Authors: Eidos <link23er@gmail.com>
 */

/*
 * ## In this file we will have several definitions on SVSM stuffs.
 * - SVSM REQUEST
 * - SVSM FUNCTION
 * - SVSM IOCTL REQUEST
 * - RAX OF SVSM REQUEST
 */
#ifndef BASIC_H
#define BASIC_H

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <inttypes.h>

// PART1: SVSM REQUEST
// ##############################################################
/* 
 * ## 1. SVSM REQUEST
 * Form of parameters convey, Derived From SVSM draft specification:
 * - Core protocol only change these 5 registers in VMSA.
 * - As for generality, other protocols here should also use 5 registers in VMSA.
 */
struct svsm_service_request {
    unsigned long rax;
    unsigned long rcx;
    unsigned long rdx;
    unsigned long r8;
    unsigned long r9;
};



// PART2: SVSM FUNCTION.
// ##############################################################

#define DEVICE_PATH                                 "/dev/vmpl_sgx_driver"


// Linux generic TEE subsystem magic number defined in <linux/tee.h>
#define SVSM_SGX_IOC_MAGIC                          0xa4

// Wrapper for SVSM Protocols.
#define __MIX__(hi, lo)                             (((long int) hi << 32) | (long int) lo)

/*
 * ## 2. SVSM FUNCTION
 * A SVSM FUNCTION is a combination of the protocol and callid.
 * The value of rax in SVSM REQUEST of a VMSA reflects the SVSM_FUNCTION.
 * - protocol: CORE PROTOCOL of SVSM; SGX PROTOCOL of NestedSGX.
 * - callid: specification instruction provided within the protocol.
 */
#define SVSM_FUNCTION(protocol, callid)             __MIX__(protocol, callid)
#define SVSM_QUERY_PROTOCOL(protocol, version)      __MIX__(protocol, version)


// PART3: SVSM IOCTL REQUEST    (for guest module ioctl).
// #############################################################

/*
 * The IOCTL REQUEST PARAMETERS Defined here are not necessarily 
 * corresponding to a SGX instruction, two types are defined here.
 * - Instructions: only those that need to pass through IOCTL syscall.
 * - - core protocol
 * - - A part of SGX protocol
 * - SGX Assisted paras.
 */

// Instructions.
#define CORE            0x0
#define ECREATE         0x1
#define EADD            0x2
#define EINIT           0x3
#define EENTER          0x4
#define EEXTEND         0x5
#define EREMOVE         0x6
#define EWB             0x7
#define ELDB            0x8
#define EAUG            0x22
#define TEST            0xa
#define ERESUME         0xb
#define EBLOCK          0x23
#define PERF            0xff
#define EMODPR          0x9
// SGX Assisted paras.
#define TRAMPOLINE_INIT_APP             0x11

/*
 * ## 3. SVSM IOCTL REQUEST
 * The sdk use IOCTL to interact with the Guest Kernel Module.
 * Therefore we need some macros to represent the request.
 */
#define SVSM_IOC_SEND_REQUEST(x)        _IOWR(SVSM_SGX_IOC_MAGIC, x, struct svsm_service_request)

// Instructions Request.
#define SVSM_CORE                       SVSM_IOC_SEND_REQUEST(CORE)
#define SVSM_ECREATE                    SVSM_IOC_SEND_REQUEST(ECREATE)
#define SVSM_EADD                       SVSM_IOC_SEND_REQUEST(EADD)
#define SVSM_EINIT                      SVSM_IOC_SEND_REQUEST(EINIT)
#define SVSM_EENTER                     SVSM_IOC_SEND_REQUEST(EENTER)
#define SVSM_EEXTEND                    SVSM_IOC_SEND_REQUEST(EEXTEND)
#define SVSM_TEST                       SVSM_IOC_SEND_REQUEST(TEST)
#define SVSM_ERESUME                    SVSM_IOC_SEND_REQUEST(ERESUME)
#define SVSM_EREMOVE                    SVSM_IOC_SEND_REQUEST(EREMOVE)                 
#define SVSM_EAUG                       SVSM_IOC_SEND_REQUEST(EAUG)
#define SVSM_EBLOCK                     SVSM_IOC_SEND_REQUEST(EBLOCK)
#define SVSM_EWB                        SVSM_IOC_SEND_REQUEST(EWB)
#define SVSM_PERF                       SVSM_IOC_SEND_REQUEST(PERF)
#define SVSM_ELDB                       SVSM_IOC_SEND_REQUEST(ELDB)
#define SVSM_EMODPR                     SVSM_IOC_SEND_REQUEST(EMODPR)
// SGX Assisted Request.
#define SVSM_TRAMPOLINE_INIT_APP        SVSM_IOC_SEND_REQUEST(TRAMPOLINE_INIT_APP)


// PART4: RAX of SVSM REQUEST    (for svsm request 'rax' register).
// #############################################################

/* 
 * ## 4. RAX of SVSM REQUESTs
 * Protocols of SVSM. (for svsm request 'rax' register)
 * - core protocol of SVSM.
 * - sgx protocol of SVSM.
 */

// --------------------------------------
// ### 4.1. CORE PROTOCOL of SVSM
#define SVSM_CORE_PROTOCOL              0

// core protocol callid of SVSM.
// Many of them won't be implemented here.
#define SVSM_CORE_REMAP_CA              0
#define SVSM_CORE_PVALIDATE             1
#define SVSM_CORE_CREATE_VCPU           2
#define SVSM_CORE_DELETE_VCPU           3
#define SVSM_CORE_DEPOSIT_MEM           4
#define SVSM_CORE_WITHDRAW_MEM          5
#define SVSM_CORE_QUERY_PROTOCOL        6
#define SVSM_CORE_CONFIGURE_VTOM        7

// --------------------------------------
// ### 4.2. SGX PROTOCOL of SVSM
#define SVSM_SGX_PROTOCOL               4

// sgx protocol callid of SVSM.
#define RSRV_INIT                       0x0
#define SVSM_SGX_ECREATE                0x1
#define SVSM_SGX_EADD                   0x2
#define SVSM_SGX_EINIT                  0x3
#define SVSM_SGX_EENTER                 0x4

#define SVSM_SGX_EEXTEND                0x5
#define SVSM_SGX_EREMOVE                0x6
#define SVSM_SGX_EWB                    0x7
#define SVSM_SGX_ELDB                   0x8
#define SVSM_SGX_EMODPR                 0x9
#define SVSM_SGX_ERESUME                0xb

#define SVSM_SGX_EAUG                   0x22
#define SVSM_SGX_EBLOCK                 0x23
// --------------------------------------
// ## 5. Microbenchmark
#define TEST_TIME                       10000
#endif