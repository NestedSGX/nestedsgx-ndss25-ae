#include "Enclave_u.h"
#include <errno.h>

typedef struct ms_ocall_print_string_t {
	const char* ms_str;
} ms_ocall_print_string_t;

typedef struct ms_sgx_oc_cpuidex_t {
	int* ms_cpuinfo;
	int ms_leaf;
	int ms_subleaf;
} ms_sgx_oc_cpuidex_t;

typedef struct ms_sgx_thread_wait_untrusted_event_ocall_t {
	int ms_retval;
	const void* ms_self;
} ms_sgx_thread_wait_untrusted_event_ocall_t;

typedef struct ms_sgx_thread_set_untrusted_event_ocall_t {
	int ms_retval;
	const void* ms_waiter;
} ms_sgx_thread_set_untrusted_event_ocall_t;

typedef struct ms_sgx_thread_setwait_untrusted_events_ocall_t {
	int ms_retval;
	const void* ms_waiter;
	const void* ms_self;
} ms_sgx_thread_setwait_untrusted_events_ocall_t;

typedef struct ms_sgx_thread_set_multiple_untrusted_events_ocall_t {
	int ms_retval;
	const void** ms_waiters;
	size_t ms_total;
} ms_sgx_thread_set_multiple_untrusted_events_ocall_t;

static sgx_status_t SGX_CDECL Enclave_ocall_print_string(void* pms)
{
	ms_ocall_print_string_t* ms = SGX_CAST(ms_ocall_print_string_t*, pms);
	ocall_print_string(ms->ms_str);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_sgx_oc_cpuidex(void* pms)
{
	ms_sgx_oc_cpuidex_t* ms = SGX_CAST(ms_sgx_oc_cpuidex_t*, pms);
	sgx_oc_cpuidex(ms->ms_cpuinfo, ms->ms_leaf, ms->ms_subleaf);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_sgx_thread_wait_untrusted_event_ocall(void* pms)
{
	ms_sgx_thread_wait_untrusted_event_ocall_t* ms = SGX_CAST(ms_sgx_thread_wait_untrusted_event_ocall_t*, pms);
	ms->ms_retval = sgx_thread_wait_untrusted_event_ocall(ms->ms_self);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_sgx_thread_set_untrusted_event_ocall(void* pms)
{
	ms_sgx_thread_set_untrusted_event_ocall_t* ms = SGX_CAST(ms_sgx_thread_set_untrusted_event_ocall_t*, pms);
	ms->ms_retval = sgx_thread_set_untrusted_event_ocall(ms->ms_waiter);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_sgx_thread_setwait_untrusted_events_ocall(void* pms)
{
	ms_sgx_thread_setwait_untrusted_events_ocall_t* ms = SGX_CAST(ms_sgx_thread_setwait_untrusted_events_ocall_t*, pms);
	ms->ms_retval = sgx_thread_setwait_untrusted_events_ocall(ms->ms_waiter, ms->ms_self);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_sgx_thread_set_multiple_untrusted_events_ocall(void* pms)
{
	ms_sgx_thread_set_multiple_untrusted_events_ocall_t* ms = SGX_CAST(ms_sgx_thread_set_multiple_untrusted_events_ocall_t*, pms);
	ms->ms_retval = sgx_thread_set_multiple_untrusted_events_ocall(ms->ms_waiters, ms->ms_total);

	return SGX_SUCCESS;
}

static const struct {
	size_t nr_ocall;
	void * table[6];
} ocall_table_Enclave = {
	6,
	{
		(void*)Enclave_ocall_print_string,
		(void*)Enclave_sgx_oc_cpuidex,
		(void*)Enclave_sgx_thread_wait_untrusted_event_ocall,
		(void*)Enclave_sgx_thread_set_untrusted_event_ocall,
		(void*)Enclave_sgx_thread_setwait_untrusted_events_ocall,
		(void*)Enclave_sgx_thread_set_multiple_untrusted_events_ocall,
	}
};
sgx_status_t ecall_cxx14_standard_user_defined_literals(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 0, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_cxx14_tuple_via_type(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 1, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_cxx14_make_unique(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 2, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_cxx14_integral_constant(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 3, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_cxx14_integer_sequence(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 4, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_cxx14_constant_begin_end(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 5, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_cxx14_exchage(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 6, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_cxx14_is_final(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 7, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_cxx14_equal_mismatch_permutation_new_overloads(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 8, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_cxx14_heterogeneous_lookup(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 9, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_cxx14_function_return_type_deduction(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 10, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_cxx14_variable_template(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 11, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_cxx14_binary_literals(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 12, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_cxx14_digit_separators(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 13, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_cxx14_generic_lambdas(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 14, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_cxx14_lambda_capture_expression(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 15, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_cxx14_attribute_deprecated(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 16, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_cxx14_aggregate_member_init(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 17, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_cxx14_relaxed_constexpr(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 18, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_cxx14_alternate_type_deduction(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 19, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_cxx14_quoted(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 20, &ocall_table_Enclave, NULL);
	return status;
}

