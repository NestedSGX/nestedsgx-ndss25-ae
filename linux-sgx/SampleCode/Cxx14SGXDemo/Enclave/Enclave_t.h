#ifndef ENCLAVE_T_H__
#define ENCLAVE_T_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include "sgx_edger8r.h" /* for sgx_ocall etc. */


#include <stdlib.h> /* for size_t */

#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _sgx_edl_timespec
#define _sgx_edl_timespec
typedef struct sgx_edl_timespec {
	int64_t sec;
	int64_t nsec;
} sgx_edl_timespec;
#endif

void ecall_cxx14_standard_user_defined_literals(void);
void ecall_cxx14_tuple_via_type(void);
void ecall_cxx14_make_unique(void);
void ecall_cxx14_integral_constant(void);
void ecall_cxx14_integer_sequence(void);
void ecall_cxx14_constant_begin_end(void);
void ecall_cxx14_exchage(void);
void ecall_cxx14_is_final(void);
void ecall_cxx14_equal_mismatch_permutation_new_overloads(void);
void ecall_cxx14_heterogeneous_lookup(void);
void ecall_cxx14_function_return_type_deduction(void);
void ecall_cxx14_variable_template(void);
void ecall_cxx14_binary_literals(void);
void ecall_cxx14_digit_separators(void);
void ecall_cxx14_generic_lambdas(void);
void ecall_cxx14_lambda_capture_expression(void);
void ecall_cxx14_attribute_deprecated(void);
void ecall_cxx14_aggregate_member_init(void);
void ecall_cxx14_relaxed_constexpr(void);
void ecall_cxx14_alternate_type_deduction(void);
void ecall_cxx14_quoted(void);

sgx_status_t SGX_CDECL ocall_print_string(const char* str);
sgx_status_t SGX_CDECL sgx_oc_cpuidex(int cpuinfo[4], int leaf, int subleaf);
sgx_status_t SGX_CDECL sgx_thread_wait_untrusted_event_ocall(int* retval, const void* self);
sgx_status_t SGX_CDECL sgx_thread_set_untrusted_event_ocall(int* retval, const void* waiter);
sgx_status_t SGX_CDECL sgx_thread_setwait_untrusted_events_ocall(int* retval, const void* waiter, const void* self);
sgx_status_t SGX_CDECL sgx_thread_set_multiple_untrusted_events_ocall(int* retval, const void** waiters, size_t total);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
