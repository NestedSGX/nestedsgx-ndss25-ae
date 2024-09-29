#include "Enclave_t.h"

#include "sgx_trts.h" /* for sgx_ocalloc, sgx_is_outside_enclave */
#include "sgx_lfence.h" /* for sgx_lfence */

#include <errno.h>
#include <mbusafecrt.h> /* for memcpy_s etc */
#include <stdlib.h> /* for malloc/free etc */

#define CHECK_REF_POINTER(ptr, siz) do {	\
	if (!(ptr) || ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define CHECK_UNIQUE_POINTER(ptr, siz) do {	\
	if ((ptr) && ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define CHECK_ENCLAVE_POINTER(ptr, siz) do {	\
	if ((ptr) && ! sgx_is_within_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define ADD_ASSIGN_OVERFLOW(a, b) (	\
	((a) += (b)) < (b)	\
)


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

static sgx_status_t SGX_CDECL sgx_ecall_cxx14_standard_user_defined_literals(void* pms)
{
	sgx_status_t status = SGX_SUCCESS;
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	ecall_cxx14_standard_user_defined_literals();
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_cxx14_tuple_via_type(void* pms)
{
	sgx_status_t status = SGX_SUCCESS;
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	ecall_cxx14_tuple_via_type();
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_cxx14_make_unique(void* pms)
{
	sgx_status_t status = SGX_SUCCESS;
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	ecall_cxx14_make_unique();
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_cxx14_integral_constant(void* pms)
{
	sgx_status_t status = SGX_SUCCESS;
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	ecall_cxx14_integral_constant();
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_cxx14_integer_sequence(void* pms)
{
	sgx_status_t status = SGX_SUCCESS;
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	ecall_cxx14_integer_sequence();
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_cxx14_constant_begin_end(void* pms)
{
	sgx_status_t status = SGX_SUCCESS;
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	ecall_cxx14_constant_begin_end();
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_cxx14_exchage(void* pms)
{
	sgx_status_t status = SGX_SUCCESS;
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	ecall_cxx14_exchage();
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_cxx14_is_final(void* pms)
{
	sgx_status_t status = SGX_SUCCESS;
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	ecall_cxx14_is_final();
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_cxx14_equal_mismatch_permutation_new_overloads(void* pms)
{
	sgx_status_t status = SGX_SUCCESS;
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	ecall_cxx14_equal_mismatch_permutation_new_overloads();
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_cxx14_heterogeneous_lookup(void* pms)
{
	sgx_status_t status = SGX_SUCCESS;
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	ecall_cxx14_heterogeneous_lookup();
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_cxx14_function_return_type_deduction(void* pms)
{
	sgx_status_t status = SGX_SUCCESS;
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	ecall_cxx14_function_return_type_deduction();
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_cxx14_variable_template(void* pms)
{
	sgx_status_t status = SGX_SUCCESS;
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	ecall_cxx14_variable_template();
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_cxx14_binary_literals(void* pms)
{
	sgx_status_t status = SGX_SUCCESS;
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	ecall_cxx14_binary_literals();
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_cxx14_digit_separators(void* pms)
{
	sgx_status_t status = SGX_SUCCESS;
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	ecall_cxx14_digit_separators();
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_cxx14_generic_lambdas(void* pms)
{
	sgx_status_t status = SGX_SUCCESS;
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	ecall_cxx14_generic_lambdas();
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_cxx14_lambda_capture_expression(void* pms)
{
	sgx_status_t status = SGX_SUCCESS;
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	ecall_cxx14_lambda_capture_expression();
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_cxx14_attribute_deprecated(void* pms)
{
	sgx_status_t status = SGX_SUCCESS;
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	ecall_cxx14_attribute_deprecated();
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_cxx14_aggregate_member_init(void* pms)
{
	sgx_status_t status = SGX_SUCCESS;
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	ecall_cxx14_aggregate_member_init();
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_cxx14_relaxed_constexpr(void* pms)
{
	sgx_status_t status = SGX_SUCCESS;
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	ecall_cxx14_relaxed_constexpr();
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_cxx14_alternate_type_deduction(void* pms)
{
	sgx_status_t status = SGX_SUCCESS;
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	ecall_cxx14_alternate_type_deduction();
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_cxx14_quoted(void* pms)
{
	sgx_status_t status = SGX_SUCCESS;
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	ecall_cxx14_quoted();
	return status;
}

SGX_EXTERNC const struct {
	size_t nr_ecall;
	struct {void* ecall_addr; uint8_t is_priv; uint8_t is_switchless;} ecall_table[21];
} g_ecall_table = {
	21,
	{
		{(void*)(uintptr_t)sgx_ecall_cxx14_standard_user_defined_literals, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_cxx14_tuple_via_type, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_cxx14_make_unique, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_cxx14_integral_constant, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_cxx14_integer_sequence, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_cxx14_constant_begin_end, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_cxx14_exchage, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_cxx14_is_final, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_cxx14_equal_mismatch_permutation_new_overloads, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_cxx14_heterogeneous_lookup, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_cxx14_function_return_type_deduction, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_cxx14_variable_template, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_cxx14_binary_literals, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_cxx14_digit_separators, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_cxx14_generic_lambdas, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_cxx14_lambda_capture_expression, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_cxx14_attribute_deprecated, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_cxx14_aggregate_member_init, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_cxx14_relaxed_constexpr, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_cxx14_alternate_type_deduction, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_cxx14_quoted, 0, 0},
	}
};

SGX_EXTERNC const struct {
	size_t nr_ocall;
	uint8_t entry_table[6][21];
} g_dyn_entry_table = {
	6,
	{
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
	}
};


sgx_status_t SGX_CDECL ocall_print_string(const char* str)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_str = str ? strlen(str) + 1 : 0;

	ms_ocall_print_string_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_print_string_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(str, _len_str);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (str != NULL) ? _len_str : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_print_string_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_print_string_t));
	ocalloc_size -= sizeof(ms_ocall_print_string_t);

	if (str != NULL) {
		if (memcpy_verw_s(&ms->ms_str, sizeof(const char*), &__tmp, sizeof(const char*))) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		if (_len_str % sizeof(*str) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_verw_s(__tmp, ocalloc_size, str, _len_str)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_str);
		ocalloc_size -= _len_str;
	} else {
		ms->ms_str = NULL;
	}

	status = sgx_ocall(0, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_oc_cpuidex(int cpuinfo[4], int leaf, int subleaf)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_cpuinfo = 4 * sizeof(int);

	ms_sgx_oc_cpuidex_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_oc_cpuidex_t);
	void *__tmp = NULL;

	void *__tmp_cpuinfo = NULL;

	CHECK_ENCLAVE_POINTER(cpuinfo, _len_cpuinfo);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (cpuinfo != NULL) ? _len_cpuinfo : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_oc_cpuidex_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_oc_cpuidex_t));
	ocalloc_size -= sizeof(ms_sgx_oc_cpuidex_t);

	if (cpuinfo != NULL) {
		if (memcpy_verw_s(&ms->ms_cpuinfo, sizeof(int*), &__tmp, sizeof(int*))) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp_cpuinfo = __tmp;
		if (_len_cpuinfo % sizeof(*cpuinfo) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		memset_verw(__tmp_cpuinfo, 0, _len_cpuinfo);
		__tmp = (void *)((size_t)__tmp + _len_cpuinfo);
		ocalloc_size -= _len_cpuinfo;
	} else {
		ms->ms_cpuinfo = NULL;
	}

	if (memcpy_verw_s(&ms->ms_leaf, sizeof(ms->ms_leaf), &leaf, sizeof(leaf))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	if (memcpy_verw_s(&ms->ms_subleaf, sizeof(ms->ms_subleaf), &subleaf, sizeof(subleaf))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	status = sgx_ocall(1, ms);

	if (status == SGX_SUCCESS) {
		if (cpuinfo) {
			if (memcpy_s((void*)cpuinfo, _len_cpuinfo, __tmp_cpuinfo, _len_cpuinfo)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_thread_wait_untrusted_event_ocall(int* retval, const void* self)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_sgx_thread_wait_untrusted_event_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_thread_wait_untrusted_event_ocall_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_thread_wait_untrusted_event_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_thread_wait_untrusted_event_ocall_t));
	ocalloc_size -= sizeof(ms_sgx_thread_wait_untrusted_event_ocall_t);

	if (memcpy_verw_s(&ms->ms_self, sizeof(ms->ms_self), &self, sizeof(self))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	status = sgx_ocall(2, ms);

	if (status == SGX_SUCCESS) {
		if (retval) {
			if (memcpy_s((void*)retval, sizeof(*retval), &ms->ms_retval, sizeof(ms->ms_retval))) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_thread_set_untrusted_event_ocall(int* retval, const void* waiter)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_sgx_thread_set_untrusted_event_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_thread_set_untrusted_event_ocall_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_thread_set_untrusted_event_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_thread_set_untrusted_event_ocall_t));
	ocalloc_size -= sizeof(ms_sgx_thread_set_untrusted_event_ocall_t);

	if (memcpy_verw_s(&ms->ms_waiter, sizeof(ms->ms_waiter), &waiter, sizeof(waiter))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	status = sgx_ocall(3, ms);

	if (status == SGX_SUCCESS) {
		if (retval) {
			if (memcpy_s((void*)retval, sizeof(*retval), &ms->ms_retval, sizeof(ms->ms_retval))) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_thread_setwait_untrusted_events_ocall(int* retval, const void* waiter, const void* self)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_sgx_thread_setwait_untrusted_events_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_thread_setwait_untrusted_events_ocall_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_thread_setwait_untrusted_events_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_thread_setwait_untrusted_events_ocall_t));
	ocalloc_size -= sizeof(ms_sgx_thread_setwait_untrusted_events_ocall_t);

	if (memcpy_verw_s(&ms->ms_waiter, sizeof(ms->ms_waiter), &waiter, sizeof(waiter))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	if (memcpy_verw_s(&ms->ms_self, sizeof(ms->ms_self), &self, sizeof(self))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	status = sgx_ocall(4, ms);

	if (status == SGX_SUCCESS) {
		if (retval) {
			if (memcpy_s((void*)retval, sizeof(*retval), &ms->ms_retval, sizeof(ms->ms_retval))) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_thread_set_multiple_untrusted_events_ocall(int* retval, const void** waiters, size_t total)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_waiters = total * sizeof(void*);

	ms_sgx_thread_set_multiple_untrusted_events_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_thread_set_multiple_untrusted_events_ocall_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(waiters, _len_waiters);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (waiters != NULL) ? _len_waiters : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_thread_set_multiple_untrusted_events_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_thread_set_multiple_untrusted_events_ocall_t));
	ocalloc_size -= sizeof(ms_sgx_thread_set_multiple_untrusted_events_ocall_t);

	if (waiters != NULL) {
		if (memcpy_verw_s(&ms->ms_waiters, sizeof(const void**), &__tmp, sizeof(const void**))) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		if (_len_waiters % sizeof(*waiters) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_verw_s(__tmp, ocalloc_size, waiters, _len_waiters)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_waiters);
		ocalloc_size -= _len_waiters;
	} else {
		ms->ms_waiters = NULL;
	}

	if (memcpy_verw_s(&ms->ms_total, sizeof(ms->ms_total), &total, sizeof(total))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	status = sgx_ocall(5, ms);

	if (status == SGX_SUCCESS) {
		if (retval) {
			if (memcpy_s((void*)retval, sizeof(*retval), &ms->ms_retval, sizeof(ms->ms_retval))) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

