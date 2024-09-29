#ifndef ENCLAVE_U_H__
#define ENCLAVE_U_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include <string.h>
#include "sgx_edger8r.h" /* for sgx_status_t etc. */


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

#ifndef OCALL_PRINT_STRING_DEFINED__
#define OCALL_PRINT_STRING_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_print_string, (const char* str));
#endif
#ifndef SGX_OC_CPUIDEX_DEFINED__
#define SGX_OC_CPUIDEX_DEFINED__
void SGX_UBRIDGE(SGX_CDECL, sgx_oc_cpuidex, (int cpuinfo[4], int leaf, int subleaf));
#endif
#ifndef SGX_THREAD_WAIT_UNTRUSTED_EVENT_OCALL_DEFINED__
#define SGX_THREAD_WAIT_UNTRUSTED_EVENT_OCALL_DEFINED__
int SGX_UBRIDGE(SGX_CDECL, sgx_thread_wait_untrusted_event_ocall, (const void* self));
#endif
#ifndef SGX_THREAD_SET_UNTRUSTED_EVENT_OCALL_DEFINED__
#define SGX_THREAD_SET_UNTRUSTED_EVENT_OCALL_DEFINED__
int SGX_UBRIDGE(SGX_CDECL, sgx_thread_set_untrusted_event_ocall, (const void* waiter));
#endif
#ifndef SGX_THREAD_SETWAIT_UNTRUSTED_EVENTS_OCALL_DEFINED__
#define SGX_THREAD_SETWAIT_UNTRUSTED_EVENTS_OCALL_DEFINED__
int SGX_UBRIDGE(SGX_CDECL, sgx_thread_setwait_untrusted_events_ocall, (const void* waiter, const void* self));
#endif
#ifndef SGX_THREAD_SET_MULTIPLE_UNTRUSTED_EVENTS_OCALL_DEFINED__
#define SGX_THREAD_SET_MULTIPLE_UNTRUSTED_EVENTS_OCALL_DEFINED__
int SGX_UBRIDGE(SGX_CDECL, sgx_thread_set_multiple_untrusted_events_ocall, (const void** waiters, size_t total));
#endif

sgx_status_t ecall_cxx14_standard_user_defined_literals(sgx_enclave_id_t eid);
sgx_status_t ecall_cxx14_tuple_via_type(sgx_enclave_id_t eid);
sgx_status_t ecall_cxx14_make_unique(sgx_enclave_id_t eid);
sgx_status_t ecall_cxx14_integral_constant(sgx_enclave_id_t eid);
sgx_status_t ecall_cxx14_integer_sequence(sgx_enclave_id_t eid);
sgx_status_t ecall_cxx14_constant_begin_end(sgx_enclave_id_t eid);
sgx_status_t ecall_cxx14_exchage(sgx_enclave_id_t eid);
sgx_status_t ecall_cxx14_is_final(sgx_enclave_id_t eid);
sgx_status_t ecall_cxx14_equal_mismatch_permutation_new_overloads(sgx_enclave_id_t eid);
sgx_status_t ecall_cxx14_heterogeneous_lookup(sgx_enclave_id_t eid);
sgx_status_t ecall_cxx14_function_return_type_deduction(sgx_enclave_id_t eid);
sgx_status_t ecall_cxx14_variable_template(sgx_enclave_id_t eid);
sgx_status_t ecall_cxx14_binary_literals(sgx_enclave_id_t eid);
sgx_status_t ecall_cxx14_digit_separators(sgx_enclave_id_t eid);
sgx_status_t ecall_cxx14_generic_lambdas(sgx_enclave_id_t eid);
sgx_status_t ecall_cxx14_lambda_capture_expression(sgx_enclave_id_t eid);
sgx_status_t ecall_cxx14_attribute_deprecated(sgx_enclave_id_t eid);
sgx_status_t ecall_cxx14_aggregate_member_init(sgx_enclave_id_t eid);
sgx_status_t ecall_cxx14_relaxed_constexpr(sgx_enclave_id_t eid);
sgx_status_t ecall_cxx14_alternate_type_deduction(sgx_enclave_id_t eid);
sgx_status_t ecall_cxx14_quoted(sgx_enclave_id_t eid);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
