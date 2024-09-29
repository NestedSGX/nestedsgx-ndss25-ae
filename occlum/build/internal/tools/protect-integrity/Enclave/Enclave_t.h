#ifndef ENCLAVE_T_H__
#define ENCLAVE_T_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include "sgx_edger8r.h" /* for sgx_ocall etc. */

#include "sys/types.h"
#include "sgx_key.h"

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

int ecall_protect(const char* input_path, const char* output_path);
int ecall_show(const char* input_path, const char* output_path);
int ecall_show_mac(const char* input_path);

sgx_status_t SGX_CDECL ocall_open_for_write(int* retval, const char* path);
sgx_status_t SGX_CDECL ocall_open_for_read(int* retval, const char* path);
sgx_status_t SGX_CDECL ocall_read(ssize_t* retval, int fd, void* buf, size_t size);
sgx_status_t SGX_CDECL ocall_write(ssize_t* retval, int fd, const void* buf, size_t size);
sgx_status_t SGX_CDECL ocall_close(int* retval, int fd);
sgx_status_t SGX_CDECL ocall_print(const char* msg);
sgx_status_t SGX_CDECL ocall_eprint(const char* msg);
sgx_status_t SGX_CDECL sgx_oc_cpuidex(int cpuinfo[4], int leaf, int subleaf);
sgx_status_t SGX_CDECL sgx_thread_wait_untrusted_event_ocall(int* retval, const void* self);
sgx_status_t SGX_CDECL sgx_thread_set_untrusted_event_ocall(int* retval, const void* waiter);
sgx_status_t SGX_CDECL sgx_thread_setwait_untrusted_events_ocall(int* retval, const void* waiter, const void* self);
sgx_status_t SGX_CDECL sgx_thread_set_multiple_untrusted_events_ocall(int* retval, const void** waiters, size_t total);
sgx_status_t SGX_CDECL u_sgxprotectedfs_exclusive_file_open(void** retval, const char* filename, uint8_t read_only, int64_t* file_size, int32_t* error_code);
sgx_status_t SGX_CDECL u_sgxprotectedfs_check_if_file_exists(uint8_t* retval, const char* filename);
sgx_status_t SGX_CDECL u_sgxprotectedfs_fread_node(int32_t* retval, void* f, uint64_t node_number, uint8_t* buffer, uint32_t node_size);
sgx_status_t SGX_CDECL u_sgxprotectedfs_fwrite_node(int32_t* retval, void* f, uint64_t node_number, uint8_t* buffer, uint32_t node_size);
sgx_status_t SGX_CDECL u_sgxprotectedfs_fclose(int32_t* retval, void* f);
sgx_status_t SGX_CDECL u_sgxprotectedfs_fflush(uint8_t* retval, void* f);
sgx_status_t SGX_CDECL u_sgxprotectedfs_remove(int32_t* retval, const char* filename);
sgx_status_t SGX_CDECL u_sgxprotectedfs_recovery_file_open(void** retval, const char* filename);
sgx_status_t SGX_CDECL u_sgxprotectedfs_fwrite_recovery_node(uint8_t* retval, void* f, uint8_t* data, uint32_t data_length);
sgx_status_t SGX_CDECL u_sgxprotectedfs_do_file_recovery(int32_t* retval, const char* filename, const char* recovery_filename, uint32_t node_size);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
