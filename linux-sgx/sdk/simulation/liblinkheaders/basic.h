/* SPDX-License-Identifier: MIT */


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
#define EENTER          0x4
#define FSGS            0x5
#define TEST            0xa
#define ERESUME         0xb

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
#define SVSM_EENTER                     SVSM_IOC_SEND_REQUEST(EENTER)
#define SVSM_FSGS                       SVSM_IOC_SEND_REQUEST(FSGS)
#define SVSM_TEST                       SVSM_IOC_SEND_REQUEST(TEST)
#define SVSM_ERESUME                    SVSM_IOC_SEND_REQUEST(ERESUME)

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
#define RSRV_INIT                       0
#define SVSM_SGX_ECREATE                1

#define SVSM_SGX_EENTER                 4

#define SVSM_SGX_FSGSREADY              5
#define SVSM_SGX_ERESUME                0xb
#endif