#include "Enclave_u.h"
#include <errno.h>

typedef struct ms_ecall_file_open_t {
	size_t ms_retval;
	int32_t* ms_error;
	const char* ms_path;
	size_t ms_path_len;
	uint8_t ms_create;
	uint8_t ms_protect_integrity;
	const sgx_key_128bit_t* ms_key;
} ms_ecall_file_open_t;

typedef struct ms_ecall_file_close_t {
	int ms_retval;
	size_t ms_file;
} ms_ecall_file_close_t;

typedef struct ms_ecall_file_flush_t {
	int ms_retval;
	size_t ms_file;
} ms_ecall_file_flush_t;

typedef struct ms_ecall_file_read_at_t {
	size_t ms_retval;
	size_t ms_file;
	size_t ms_offset;
	uint8_t* ms_buf;
	size_t ms_len;
} ms_ecall_file_read_at_t;

typedef struct ms_ecall_file_write_at_t {
	size_t ms_retval;
	size_t ms_file;
	size_t ms_offset;
	const uint8_t* ms_buf;
	size_t ms_len;
} ms_ecall_file_write_at_t;

typedef struct ms_ecall_file_get_mac_t {
	int ms_retval;
	size_t ms_file;
	uint8_t* ms_buf;
	size_t ms_len;
} ms_ecall_file_get_mac_t;

typedef struct ms_u_read_ocall_t {
	size_t ms_retval;
	int* ms_error;
	int ms_fd;
	void* ms_buf;
	size_t ms_count;
} ms_u_read_ocall_t;

typedef struct ms_u_pread64_ocall_t {
	size_t ms_retval;
	int* ms_error;
	int ms_fd;
	void* ms_buf;
	size_t ms_count;
	int64_t ms_offset;
} ms_u_pread64_ocall_t;

typedef struct ms_u_readv_ocall_t {
	size_t ms_retval;
	int* ms_error;
	int ms_fd;
	const struct iovec* ms_iov;
	int ms_iovcnt;
} ms_u_readv_ocall_t;

typedef struct ms_u_preadv64_ocall_t {
	size_t ms_retval;
	int* ms_error;
	int ms_fd;
	const struct iovec* ms_iov;
	int ms_iovcnt;
	int64_t ms_offset;
} ms_u_preadv64_ocall_t;

typedef struct ms_u_write_ocall_t {
	size_t ms_retval;
	int* ms_error;
	int ms_fd;
	const void* ms_buf;
	size_t ms_count;
} ms_u_write_ocall_t;

typedef struct ms_u_pwrite64_ocall_t {
	size_t ms_retval;
	int* ms_error;
	int ms_fd;
	const void* ms_buf;
	size_t ms_count;
	int64_t ms_offset;
} ms_u_pwrite64_ocall_t;

typedef struct ms_u_writev_ocall_t {
	size_t ms_retval;
	int* ms_error;
	int ms_fd;
	const struct iovec* ms_iov;
	int ms_iovcnt;
} ms_u_writev_ocall_t;

typedef struct ms_u_pwritev64_ocall_t {
	size_t ms_retval;
	int* ms_error;
	int ms_fd;
	const struct iovec* ms_iov;
	int ms_iovcnt;
	int64_t ms_offset;
} ms_u_pwritev64_ocall_t;

typedef struct ms_u_fcntl_arg0_ocall_t {
	int ms_retval;
	int* ms_error;
	int ms_fd;
	int ms_cmd;
} ms_u_fcntl_arg0_ocall_t;

typedef struct ms_u_fcntl_arg1_ocall_t {
	int ms_retval;
	int* ms_error;
	int ms_fd;
	int ms_cmd;
	int ms_arg;
} ms_u_fcntl_arg1_ocall_t;

typedef struct ms_u_ioctl_arg0_ocall_t {
	int ms_retval;
	int* ms_error;
	int ms_fd;
	int ms_request;
} ms_u_ioctl_arg0_ocall_t;

typedef struct ms_u_ioctl_arg1_ocall_t {
	int ms_retval;
	int* ms_error;
	int ms_fd;
	int ms_request;
	int* ms_arg;
} ms_u_ioctl_arg1_ocall_t;

typedef struct ms_u_close_ocall_t {
	int ms_retval;
	int* ms_error;
	int ms_fd;
} ms_u_close_ocall_t;

typedef struct ms_u_malloc_ocall_t {
	void* ms_retval;
	int* ms_error;
	size_t ms_size;
} ms_u_malloc_ocall_t;

typedef struct ms_u_free_ocall_t {
	void* ms_p;
} ms_u_free_ocall_t;

typedef struct ms_u_mmap_ocall_t {
	void* ms_retval;
	int* ms_error;
	void* ms_start;
	size_t ms_length;
	int ms_prot;
	int ms_flags;
	int ms_fd;
	int64_t ms_offset;
} ms_u_mmap_ocall_t;

typedef struct ms_u_munmap_ocall_t {
	int ms_retval;
	int* ms_error;
	void* ms_start;
	size_t ms_length;
} ms_u_munmap_ocall_t;

typedef struct ms_u_msync_ocall_t {
	int ms_retval;
	int* ms_error;
	void* ms_addr;
	size_t ms_length;
	int ms_flags;
} ms_u_msync_ocall_t;

typedef struct ms_u_mprotect_ocall_t {
	int ms_retval;
	int* ms_error;
	void* ms_addr;
	size_t ms_length;
	int ms_prot;
} ms_u_mprotect_ocall_t;

typedef struct ms_u_open_ocall_t {
	int ms_retval;
	int* ms_error;
	const char* ms_pathname;
	int ms_flags;
} ms_u_open_ocall_t;

typedef struct ms_u_open64_ocall_t {
	int ms_retval;
	int* ms_error;
	const char* ms_path;
	int ms_oflag;
	int ms_mode;
} ms_u_open64_ocall_t;

typedef struct ms_u_openat_ocall_t {
	int ms_retval;
	int* ms_error;
	int ms_dirfd;
	const char* ms_pathname;
	int ms_flags;
} ms_u_openat_ocall_t;

typedef struct ms_u_fstat_ocall_t {
	int ms_retval;
	int* ms_error;
	int ms_fd;
	struct stat_t* ms_buf;
} ms_u_fstat_ocall_t;

typedef struct ms_u_fstat64_ocall_t {
	int ms_retval;
	int* ms_error;
	int ms_fd;
	struct stat64_t* ms_buf;
} ms_u_fstat64_ocall_t;

typedef struct ms_u_stat_ocall_t {
	int ms_retval;
	int* ms_error;
	const char* ms_path;
	struct stat_t* ms_buf;
} ms_u_stat_ocall_t;

typedef struct ms_u_stat64_ocall_t {
	int ms_retval;
	int* ms_error;
	const char* ms_path;
	struct stat64_t* ms_buf;
} ms_u_stat64_ocall_t;

typedef struct ms_u_lstat_ocall_t {
	int ms_retval;
	int* ms_error;
	const char* ms_path;
	struct stat_t* ms_buf;
} ms_u_lstat_ocall_t;

typedef struct ms_u_lstat64_ocall_t {
	int ms_retval;
	int* ms_error;
	const char* ms_path;
	struct stat64_t* ms_buf;
} ms_u_lstat64_ocall_t;

typedef struct ms_u_lseek_ocall_t {
	uint64_t ms_retval;
	int* ms_error;
	int ms_fd;
	int64_t ms_offset;
	int ms_whence;
} ms_u_lseek_ocall_t;

typedef struct ms_u_lseek64_ocall_t {
	int64_t ms_retval;
	int* ms_error;
	int ms_fd;
	int64_t ms_offset;
	int ms_whence;
} ms_u_lseek64_ocall_t;

typedef struct ms_u_ftruncate_ocall_t {
	int ms_retval;
	int* ms_error;
	int ms_fd;
	int64_t ms_length;
} ms_u_ftruncate_ocall_t;

typedef struct ms_u_ftruncate64_ocall_t {
	int ms_retval;
	int* ms_error;
	int ms_fd;
	int64_t ms_length;
} ms_u_ftruncate64_ocall_t;

typedef struct ms_u_truncate_ocall_t {
	int ms_retval;
	int* ms_error;
	const char* ms_path;
	int64_t ms_length;
} ms_u_truncate_ocall_t;

typedef struct ms_u_truncate64_ocall_t {
	int ms_retval;
	int* ms_error;
	const char* ms_path;
	int64_t ms_length;
} ms_u_truncate64_ocall_t;

typedef struct ms_u_fsync_ocall_t {
	int ms_retval;
	int* ms_error;
	int ms_fd;
} ms_u_fsync_ocall_t;

typedef struct ms_u_fdatasync_ocall_t {
	int ms_retval;
	int* ms_error;
	int ms_fd;
} ms_u_fdatasync_ocall_t;

typedef struct ms_u_fchmod_ocall_t {
	int ms_retval;
	int* ms_error;
	int ms_fd;
	uint32_t ms_mode;
} ms_u_fchmod_ocall_t;

typedef struct ms_u_unlink_ocall_t {
	int ms_retval;
	int* ms_error;
	const char* ms_pathname;
} ms_u_unlink_ocall_t;

typedef struct ms_u_link_ocall_t {
	int ms_retval;
	int* ms_error;
	const char* ms_oldpath;
	const char* ms_newpath;
} ms_u_link_ocall_t;

typedef struct ms_u_unlinkat_ocall_t {
	int ms_retval;
	int* ms_error;
	int ms_dirfd;
	const char* ms_pathname;
	int ms_flags;
} ms_u_unlinkat_ocall_t;

typedef struct ms_u_linkat_ocall_t {
	int ms_retval;
	int* ms_error;
	int ms_olddirfd;
	const char* ms_oldpath;
	int ms_newdirfd;
	const char* ms_newpath;
	int ms_flags;
} ms_u_linkat_ocall_t;

typedef struct ms_u_rename_ocall_t {
	int ms_retval;
	int* ms_error;
	const char* ms_oldpath;
	const char* ms_newpath;
} ms_u_rename_ocall_t;

typedef struct ms_u_chmod_ocall_t {
	int ms_retval;
	int* ms_error;
	const char* ms_path;
	uint32_t ms_mode;
} ms_u_chmod_ocall_t;

typedef struct ms_u_readlink_ocall_t {
	size_t ms_retval;
	int* ms_error;
	const char* ms_path;
	char* ms_buf;
	size_t ms_bufsz;
} ms_u_readlink_ocall_t;

typedef struct ms_u_symlink_ocall_t {
	int ms_retval;
	int* ms_error;
	const char* ms_path1;
	const char* ms_path2;
} ms_u_symlink_ocall_t;

typedef struct ms_u_realpath_ocall_t {
	char* ms_retval;
	int* ms_error;
	const char* ms_pathname;
} ms_u_realpath_ocall_t;

typedef struct ms_u_mkdir_ocall_t {
	int ms_retval;
	int* ms_error;
	const char* ms_pathname;
	uint32_t ms_mode;
} ms_u_mkdir_ocall_t;

typedef struct ms_u_rmdir_ocall_t {
	int ms_retval;
	int* ms_error;
	const char* ms_pathname;
} ms_u_rmdir_ocall_t;

typedef struct ms_u_fdopendir_ocall_t {
	void* ms_retval;
	int* ms_error;
	int ms_fd;
} ms_u_fdopendir_ocall_t;

typedef struct ms_u_opendir_ocall_t {
	void* ms_retval;
	int* ms_error;
	const char* ms_pathname;
} ms_u_opendir_ocall_t;

typedef struct ms_u_readdir64_r_ocall_t {
	int ms_retval;
	void* ms_dirp;
	struct dirent64_t* ms_entry;
	struct dirent64_t** ms_result;
} ms_u_readdir64_r_ocall_t;

typedef struct ms_u_closedir_ocall_t {
	int ms_retval;
	int* ms_error;
	void* ms_dirp;
} ms_u_closedir_ocall_t;

typedef struct ms_u_dirfd_ocall_t {
	int ms_retval;
	int* ms_error;
	void* ms_dirp;
} ms_u_dirfd_ocall_t;

typedef struct ms_u_fstatat64_ocall_t {
	int ms_retval;
	int* ms_error;
	int ms_dirfd;
	const char* ms_pathname;
	struct stat64_t* ms_buf;
	int ms_flags;
} ms_u_fstatat64_ocall_t;

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

typedef struct ms_u_sgxprotectedfs_exclusive_file_open_t {
	void* ms_retval;
	const char* ms_filename;
	uint8_t ms_read_only;
	int64_t* ms_file_size;
	int32_t* ms_error_code;
} ms_u_sgxprotectedfs_exclusive_file_open_t;

typedef struct ms_u_sgxprotectedfs_check_if_file_exists_t {
	uint8_t ms_retval;
	const char* ms_filename;
} ms_u_sgxprotectedfs_check_if_file_exists_t;

typedef struct ms_u_sgxprotectedfs_fread_node_t {
	int32_t ms_retval;
	void* ms_f;
	uint64_t ms_node_number;
	uint8_t* ms_buffer;
	uint32_t ms_node_size;
} ms_u_sgxprotectedfs_fread_node_t;

typedef struct ms_u_sgxprotectedfs_fwrite_node_t {
	int32_t ms_retval;
	void* ms_f;
	uint64_t ms_node_number;
	uint8_t* ms_buffer;
	uint32_t ms_node_size;
} ms_u_sgxprotectedfs_fwrite_node_t;

typedef struct ms_u_sgxprotectedfs_fclose_t {
	int32_t ms_retval;
	void* ms_f;
} ms_u_sgxprotectedfs_fclose_t;

typedef struct ms_u_sgxprotectedfs_fflush_t {
	uint8_t ms_retval;
	void* ms_f;
} ms_u_sgxprotectedfs_fflush_t;

typedef struct ms_u_sgxprotectedfs_remove_t {
	int32_t ms_retval;
	const char* ms_filename;
} ms_u_sgxprotectedfs_remove_t;

typedef struct ms_u_sgxprotectedfs_recovery_file_open_t {
	void* ms_retval;
	const char* ms_filename;
} ms_u_sgxprotectedfs_recovery_file_open_t;

typedef struct ms_u_sgxprotectedfs_fwrite_recovery_node_t {
	uint8_t ms_retval;
	void* ms_f;
	uint8_t* ms_data;
	uint32_t ms_data_length;
} ms_u_sgxprotectedfs_fwrite_recovery_node_t;

typedef struct ms_u_sgxprotectedfs_do_file_recovery_t {
	int32_t ms_retval;
	const char* ms_filename;
	const char* ms_recovery_filename;
	uint32_t ms_node_size;
} ms_u_sgxprotectedfs_do_file_recovery_t;

static sgx_status_t SGX_CDECL Enclave_u_read_ocall(void* pms)
{
	ms_u_read_ocall_t* ms = SGX_CAST(ms_u_read_ocall_t*, pms);
	ms->ms_retval = u_read_ocall(ms->ms_error, ms->ms_fd, ms->ms_buf, ms->ms_count);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_pread64_ocall(void* pms)
{
	ms_u_pread64_ocall_t* ms = SGX_CAST(ms_u_pread64_ocall_t*, pms);
	ms->ms_retval = u_pread64_ocall(ms->ms_error, ms->ms_fd, ms->ms_buf, ms->ms_count, ms->ms_offset);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_readv_ocall(void* pms)
{
	ms_u_readv_ocall_t* ms = SGX_CAST(ms_u_readv_ocall_t*, pms);
	ms->ms_retval = u_readv_ocall(ms->ms_error, ms->ms_fd, ms->ms_iov, ms->ms_iovcnt);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_preadv64_ocall(void* pms)
{
	ms_u_preadv64_ocall_t* ms = SGX_CAST(ms_u_preadv64_ocall_t*, pms);
	ms->ms_retval = u_preadv64_ocall(ms->ms_error, ms->ms_fd, ms->ms_iov, ms->ms_iovcnt, ms->ms_offset);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_write_ocall(void* pms)
{
	ms_u_write_ocall_t* ms = SGX_CAST(ms_u_write_ocall_t*, pms);
	ms->ms_retval = u_write_ocall(ms->ms_error, ms->ms_fd, ms->ms_buf, ms->ms_count);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_pwrite64_ocall(void* pms)
{
	ms_u_pwrite64_ocall_t* ms = SGX_CAST(ms_u_pwrite64_ocall_t*, pms);
	ms->ms_retval = u_pwrite64_ocall(ms->ms_error, ms->ms_fd, ms->ms_buf, ms->ms_count, ms->ms_offset);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_writev_ocall(void* pms)
{
	ms_u_writev_ocall_t* ms = SGX_CAST(ms_u_writev_ocall_t*, pms);
	ms->ms_retval = u_writev_ocall(ms->ms_error, ms->ms_fd, ms->ms_iov, ms->ms_iovcnt);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_pwritev64_ocall(void* pms)
{
	ms_u_pwritev64_ocall_t* ms = SGX_CAST(ms_u_pwritev64_ocall_t*, pms);
	ms->ms_retval = u_pwritev64_ocall(ms->ms_error, ms->ms_fd, ms->ms_iov, ms->ms_iovcnt, ms->ms_offset);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_fcntl_arg0_ocall(void* pms)
{
	ms_u_fcntl_arg0_ocall_t* ms = SGX_CAST(ms_u_fcntl_arg0_ocall_t*, pms);
	ms->ms_retval = u_fcntl_arg0_ocall(ms->ms_error, ms->ms_fd, ms->ms_cmd);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_fcntl_arg1_ocall(void* pms)
{
	ms_u_fcntl_arg1_ocall_t* ms = SGX_CAST(ms_u_fcntl_arg1_ocall_t*, pms);
	ms->ms_retval = u_fcntl_arg1_ocall(ms->ms_error, ms->ms_fd, ms->ms_cmd, ms->ms_arg);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_ioctl_arg0_ocall(void* pms)
{
	ms_u_ioctl_arg0_ocall_t* ms = SGX_CAST(ms_u_ioctl_arg0_ocall_t*, pms);
	ms->ms_retval = u_ioctl_arg0_ocall(ms->ms_error, ms->ms_fd, ms->ms_request);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_ioctl_arg1_ocall(void* pms)
{
	ms_u_ioctl_arg1_ocall_t* ms = SGX_CAST(ms_u_ioctl_arg1_ocall_t*, pms);
	ms->ms_retval = u_ioctl_arg1_ocall(ms->ms_error, ms->ms_fd, ms->ms_request, ms->ms_arg);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_close_ocall(void* pms)
{
	ms_u_close_ocall_t* ms = SGX_CAST(ms_u_close_ocall_t*, pms);
	ms->ms_retval = u_close_ocall(ms->ms_error, ms->ms_fd);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_malloc_ocall(void* pms)
{
	ms_u_malloc_ocall_t* ms = SGX_CAST(ms_u_malloc_ocall_t*, pms);
	ms->ms_retval = u_malloc_ocall(ms->ms_error, ms->ms_size);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_free_ocall(void* pms)
{
	ms_u_free_ocall_t* ms = SGX_CAST(ms_u_free_ocall_t*, pms);
	u_free_ocall(ms->ms_p);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_mmap_ocall(void* pms)
{
	ms_u_mmap_ocall_t* ms = SGX_CAST(ms_u_mmap_ocall_t*, pms);
	ms->ms_retval = u_mmap_ocall(ms->ms_error, ms->ms_start, ms->ms_length, ms->ms_prot, ms->ms_flags, ms->ms_fd, ms->ms_offset);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_munmap_ocall(void* pms)
{
	ms_u_munmap_ocall_t* ms = SGX_CAST(ms_u_munmap_ocall_t*, pms);
	ms->ms_retval = u_munmap_ocall(ms->ms_error, ms->ms_start, ms->ms_length);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_msync_ocall(void* pms)
{
	ms_u_msync_ocall_t* ms = SGX_CAST(ms_u_msync_ocall_t*, pms);
	ms->ms_retval = u_msync_ocall(ms->ms_error, ms->ms_addr, ms->ms_length, ms->ms_flags);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_mprotect_ocall(void* pms)
{
	ms_u_mprotect_ocall_t* ms = SGX_CAST(ms_u_mprotect_ocall_t*, pms);
	ms->ms_retval = u_mprotect_ocall(ms->ms_error, ms->ms_addr, ms->ms_length, ms->ms_prot);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_open_ocall(void* pms)
{
	ms_u_open_ocall_t* ms = SGX_CAST(ms_u_open_ocall_t*, pms);
	ms->ms_retval = u_open_ocall(ms->ms_error, ms->ms_pathname, ms->ms_flags);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_open64_ocall(void* pms)
{
	ms_u_open64_ocall_t* ms = SGX_CAST(ms_u_open64_ocall_t*, pms);
	ms->ms_retval = u_open64_ocall(ms->ms_error, ms->ms_path, ms->ms_oflag, ms->ms_mode);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_openat_ocall(void* pms)
{
	ms_u_openat_ocall_t* ms = SGX_CAST(ms_u_openat_ocall_t*, pms);
	ms->ms_retval = u_openat_ocall(ms->ms_error, ms->ms_dirfd, ms->ms_pathname, ms->ms_flags);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_fstat_ocall(void* pms)
{
	ms_u_fstat_ocall_t* ms = SGX_CAST(ms_u_fstat_ocall_t*, pms);
	ms->ms_retval = u_fstat_ocall(ms->ms_error, ms->ms_fd, ms->ms_buf);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_fstat64_ocall(void* pms)
{
	ms_u_fstat64_ocall_t* ms = SGX_CAST(ms_u_fstat64_ocall_t*, pms);
	ms->ms_retval = u_fstat64_ocall(ms->ms_error, ms->ms_fd, ms->ms_buf);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_stat_ocall(void* pms)
{
	ms_u_stat_ocall_t* ms = SGX_CAST(ms_u_stat_ocall_t*, pms);
	ms->ms_retval = u_stat_ocall(ms->ms_error, ms->ms_path, ms->ms_buf);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_stat64_ocall(void* pms)
{
	ms_u_stat64_ocall_t* ms = SGX_CAST(ms_u_stat64_ocall_t*, pms);
	ms->ms_retval = u_stat64_ocall(ms->ms_error, ms->ms_path, ms->ms_buf);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_lstat_ocall(void* pms)
{
	ms_u_lstat_ocall_t* ms = SGX_CAST(ms_u_lstat_ocall_t*, pms);
	ms->ms_retval = u_lstat_ocall(ms->ms_error, ms->ms_path, ms->ms_buf);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_lstat64_ocall(void* pms)
{
	ms_u_lstat64_ocall_t* ms = SGX_CAST(ms_u_lstat64_ocall_t*, pms);
	ms->ms_retval = u_lstat64_ocall(ms->ms_error, ms->ms_path, ms->ms_buf);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_lseek_ocall(void* pms)
{
	ms_u_lseek_ocall_t* ms = SGX_CAST(ms_u_lseek_ocall_t*, pms);
	ms->ms_retval = u_lseek_ocall(ms->ms_error, ms->ms_fd, ms->ms_offset, ms->ms_whence);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_lseek64_ocall(void* pms)
{
	ms_u_lseek64_ocall_t* ms = SGX_CAST(ms_u_lseek64_ocall_t*, pms);
	ms->ms_retval = u_lseek64_ocall(ms->ms_error, ms->ms_fd, ms->ms_offset, ms->ms_whence);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_ftruncate_ocall(void* pms)
{
	ms_u_ftruncate_ocall_t* ms = SGX_CAST(ms_u_ftruncate_ocall_t*, pms);
	ms->ms_retval = u_ftruncate_ocall(ms->ms_error, ms->ms_fd, ms->ms_length);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_ftruncate64_ocall(void* pms)
{
	ms_u_ftruncate64_ocall_t* ms = SGX_CAST(ms_u_ftruncate64_ocall_t*, pms);
	ms->ms_retval = u_ftruncate64_ocall(ms->ms_error, ms->ms_fd, ms->ms_length);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_truncate_ocall(void* pms)
{
	ms_u_truncate_ocall_t* ms = SGX_CAST(ms_u_truncate_ocall_t*, pms);
	ms->ms_retval = u_truncate_ocall(ms->ms_error, ms->ms_path, ms->ms_length);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_truncate64_ocall(void* pms)
{
	ms_u_truncate64_ocall_t* ms = SGX_CAST(ms_u_truncate64_ocall_t*, pms);
	ms->ms_retval = u_truncate64_ocall(ms->ms_error, ms->ms_path, ms->ms_length);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_fsync_ocall(void* pms)
{
	ms_u_fsync_ocall_t* ms = SGX_CAST(ms_u_fsync_ocall_t*, pms);
	ms->ms_retval = u_fsync_ocall(ms->ms_error, ms->ms_fd);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_fdatasync_ocall(void* pms)
{
	ms_u_fdatasync_ocall_t* ms = SGX_CAST(ms_u_fdatasync_ocall_t*, pms);
	ms->ms_retval = u_fdatasync_ocall(ms->ms_error, ms->ms_fd);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_fchmod_ocall(void* pms)
{
	ms_u_fchmod_ocall_t* ms = SGX_CAST(ms_u_fchmod_ocall_t*, pms);
	ms->ms_retval = u_fchmod_ocall(ms->ms_error, ms->ms_fd, ms->ms_mode);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_unlink_ocall(void* pms)
{
	ms_u_unlink_ocall_t* ms = SGX_CAST(ms_u_unlink_ocall_t*, pms);
	ms->ms_retval = u_unlink_ocall(ms->ms_error, ms->ms_pathname);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_link_ocall(void* pms)
{
	ms_u_link_ocall_t* ms = SGX_CAST(ms_u_link_ocall_t*, pms);
	ms->ms_retval = u_link_ocall(ms->ms_error, ms->ms_oldpath, ms->ms_newpath);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_unlinkat_ocall(void* pms)
{
	ms_u_unlinkat_ocall_t* ms = SGX_CAST(ms_u_unlinkat_ocall_t*, pms);
	ms->ms_retval = u_unlinkat_ocall(ms->ms_error, ms->ms_dirfd, ms->ms_pathname, ms->ms_flags);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_linkat_ocall(void* pms)
{
	ms_u_linkat_ocall_t* ms = SGX_CAST(ms_u_linkat_ocall_t*, pms);
	ms->ms_retval = u_linkat_ocall(ms->ms_error, ms->ms_olddirfd, ms->ms_oldpath, ms->ms_newdirfd, ms->ms_newpath, ms->ms_flags);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_rename_ocall(void* pms)
{
	ms_u_rename_ocall_t* ms = SGX_CAST(ms_u_rename_ocall_t*, pms);
	ms->ms_retval = u_rename_ocall(ms->ms_error, ms->ms_oldpath, ms->ms_newpath);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_chmod_ocall(void* pms)
{
	ms_u_chmod_ocall_t* ms = SGX_CAST(ms_u_chmod_ocall_t*, pms);
	ms->ms_retval = u_chmod_ocall(ms->ms_error, ms->ms_path, ms->ms_mode);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_readlink_ocall(void* pms)
{
	ms_u_readlink_ocall_t* ms = SGX_CAST(ms_u_readlink_ocall_t*, pms);
	ms->ms_retval = u_readlink_ocall(ms->ms_error, ms->ms_path, ms->ms_buf, ms->ms_bufsz);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_symlink_ocall(void* pms)
{
	ms_u_symlink_ocall_t* ms = SGX_CAST(ms_u_symlink_ocall_t*, pms);
	ms->ms_retval = u_symlink_ocall(ms->ms_error, ms->ms_path1, ms->ms_path2);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_realpath_ocall(void* pms)
{
	ms_u_realpath_ocall_t* ms = SGX_CAST(ms_u_realpath_ocall_t*, pms);
	ms->ms_retval = u_realpath_ocall(ms->ms_error, ms->ms_pathname);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_mkdir_ocall(void* pms)
{
	ms_u_mkdir_ocall_t* ms = SGX_CAST(ms_u_mkdir_ocall_t*, pms);
	ms->ms_retval = u_mkdir_ocall(ms->ms_error, ms->ms_pathname, ms->ms_mode);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_rmdir_ocall(void* pms)
{
	ms_u_rmdir_ocall_t* ms = SGX_CAST(ms_u_rmdir_ocall_t*, pms);
	ms->ms_retval = u_rmdir_ocall(ms->ms_error, ms->ms_pathname);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_fdopendir_ocall(void* pms)
{
	ms_u_fdopendir_ocall_t* ms = SGX_CAST(ms_u_fdopendir_ocall_t*, pms);
	ms->ms_retval = u_fdopendir_ocall(ms->ms_error, ms->ms_fd);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_opendir_ocall(void* pms)
{
	ms_u_opendir_ocall_t* ms = SGX_CAST(ms_u_opendir_ocall_t*, pms);
	ms->ms_retval = u_opendir_ocall(ms->ms_error, ms->ms_pathname);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_readdir64_r_ocall(void* pms)
{
	ms_u_readdir64_r_ocall_t* ms = SGX_CAST(ms_u_readdir64_r_ocall_t*, pms);
	ms->ms_retval = u_readdir64_r_ocall(ms->ms_dirp, ms->ms_entry, ms->ms_result);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_closedir_ocall(void* pms)
{
	ms_u_closedir_ocall_t* ms = SGX_CAST(ms_u_closedir_ocall_t*, pms);
	ms->ms_retval = u_closedir_ocall(ms->ms_error, ms->ms_dirp);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_dirfd_ocall(void* pms)
{
	ms_u_dirfd_ocall_t* ms = SGX_CAST(ms_u_dirfd_ocall_t*, pms);
	ms->ms_retval = u_dirfd_ocall(ms->ms_error, ms->ms_dirp);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_fstatat64_ocall(void* pms)
{
	ms_u_fstatat64_ocall_t* ms = SGX_CAST(ms_u_fstatat64_ocall_t*, pms);
	ms->ms_retval = u_fstatat64_ocall(ms->ms_error, ms->ms_dirfd, ms->ms_pathname, ms->ms_buf, ms->ms_flags);

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

static sgx_status_t SGX_CDECL Enclave_u_sgxprotectedfs_exclusive_file_open(void* pms)
{
	ms_u_sgxprotectedfs_exclusive_file_open_t* ms = SGX_CAST(ms_u_sgxprotectedfs_exclusive_file_open_t*, pms);
	ms->ms_retval = u_sgxprotectedfs_exclusive_file_open(ms->ms_filename, ms->ms_read_only, ms->ms_file_size, ms->ms_error_code);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_sgxprotectedfs_check_if_file_exists(void* pms)
{
	ms_u_sgxprotectedfs_check_if_file_exists_t* ms = SGX_CAST(ms_u_sgxprotectedfs_check_if_file_exists_t*, pms);
	ms->ms_retval = u_sgxprotectedfs_check_if_file_exists(ms->ms_filename);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_sgxprotectedfs_fread_node(void* pms)
{
	ms_u_sgxprotectedfs_fread_node_t* ms = SGX_CAST(ms_u_sgxprotectedfs_fread_node_t*, pms);
	ms->ms_retval = u_sgxprotectedfs_fread_node(ms->ms_f, ms->ms_node_number, ms->ms_buffer, ms->ms_node_size);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_sgxprotectedfs_fwrite_node(void* pms)
{
	ms_u_sgxprotectedfs_fwrite_node_t* ms = SGX_CAST(ms_u_sgxprotectedfs_fwrite_node_t*, pms);
	ms->ms_retval = u_sgxprotectedfs_fwrite_node(ms->ms_f, ms->ms_node_number, ms->ms_buffer, ms->ms_node_size);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_sgxprotectedfs_fclose(void* pms)
{
	ms_u_sgxprotectedfs_fclose_t* ms = SGX_CAST(ms_u_sgxprotectedfs_fclose_t*, pms);
	ms->ms_retval = u_sgxprotectedfs_fclose(ms->ms_f);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_sgxprotectedfs_fflush(void* pms)
{
	ms_u_sgxprotectedfs_fflush_t* ms = SGX_CAST(ms_u_sgxprotectedfs_fflush_t*, pms);
	ms->ms_retval = u_sgxprotectedfs_fflush(ms->ms_f);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_sgxprotectedfs_remove(void* pms)
{
	ms_u_sgxprotectedfs_remove_t* ms = SGX_CAST(ms_u_sgxprotectedfs_remove_t*, pms);
	ms->ms_retval = u_sgxprotectedfs_remove(ms->ms_filename);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_sgxprotectedfs_recovery_file_open(void* pms)
{
	ms_u_sgxprotectedfs_recovery_file_open_t* ms = SGX_CAST(ms_u_sgxprotectedfs_recovery_file_open_t*, pms);
	ms->ms_retval = u_sgxprotectedfs_recovery_file_open(ms->ms_filename);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_sgxprotectedfs_fwrite_recovery_node(void* pms)
{
	ms_u_sgxprotectedfs_fwrite_recovery_node_t* ms = SGX_CAST(ms_u_sgxprotectedfs_fwrite_recovery_node_t*, pms);
	ms->ms_retval = u_sgxprotectedfs_fwrite_recovery_node(ms->ms_f, ms->ms_data, ms->ms_data_length);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_u_sgxprotectedfs_do_file_recovery(void* pms)
{
	ms_u_sgxprotectedfs_do_file_recovery_t* ms = SGX_CAST(ms_u_sgxprotectedfs_do_file_recovery_t*, pms);
	ms->ms_retval = u_sgxprotectedfs_do_file_recovery(ms->ms_filename, ms->ms_recovery_filename, ms->ms_node_size);

	return SGX_SUCCESS;
}

static const struct {
	size_t nr_ocall;
	void * table[69];
} ocall_table_Enclave = {
	69,
	{
		(void*)Enclave_u_read_ocall,
		(void*)Enclave_u_pread64_ocall,
		(void*)Enclave_u_readv_ocall,
		(void*)Enclave_u_preadv64_ocall,
		(void*)Enclave_u_write_ocall,
		(void*)Enclave_u_pwrite64_ocall,
		(void*)Enclave_u_writev_ocall,
		(void*)Enclave_u_pwritev64_ocall,
		(void*)Enclave_u_fcntl_arg0_ocall,
		(void*)Enclave_u_fcntl_arg1_ocall,
		(void*)Enclave_u_ioctl_arg0_ocall,
		(void*)Enclave_u_ioctl_arg1_ocall,
		(void*)Enclave_u_close_ocall,
		(void*)Enclave_u_malloc_ocall,
		(void*)Enclave_u_free_ocall,
		(void*)Enclave_u_mmap_ocall,
		(void*)Enclave_u_munmap_ocall,
		(void*)Enclave_u_msync_ocall,
		(void*)Enclave_u_mprotect_ocall,
		(void*)Enclave_u_open_ocall,
		(void*)Enclave_u_open64_ocall,
		(void*)Enclave_u_openat_ocall,
		(void*)Enclave_u_fstat_ocall,
		(void*)Enclave_u_fstat64_ocall,
		(void*)Enclave_u_stat_ocall,
		(void*)Enclave_u_stat64_ocall,
		(void*)Enclave_u_lstat_ocall,
		(void*)Enclave_u_lstat64_ocall,
		(void*)Enclave_u_lseek_ocall,
		(void*)Enclave_u_lseek64_ocall,
		(void*)Enclave_u_ftruncate_ocall,
		(void*)Enclave_u_ftruncate64_ocall,
		(void*)Enclave_u_truncate_ocall,
		(void*)Enclave_u_truncate64_ocall,
		(void*)Enclave_u_fsync_ocall,
		(void*)Enclave_u_fdatasync_ocall,
		(void*)Enclave_u_fchmod_ocall,
		(void*)Enclave_u_unlink_ocall,
		(void*)Enclave_u_link_ocall,
		(void*)Enclave_u_unlinkat_ocall,
		(void*)Enclave_u_linkat_ocall,
		(void*)Enclave_u_rename_ocall,
		(void*)Enclave_u_chmod_ocall,
		(void*)Enclave_u_readlink_ocall,
		(void*)Enclave_u_symlink_ocall,
		(void*)Enclave_u_realpath_ocall,
		(void*)Enclave_u_mkdir_ocall,
		(void*)Enclave_u_rmdir_ocall,
		(void*)Enclave_u_fdopendir_ocall,
		(void*)Enclave_u_opendir_ocall,
		(void*)Enclave_u_readdir64_r_ocall,
		(void*)Enclave_u_closedir_ocall,
		(void*)Enclave_u_dirfd_ocall,
		(void*)Enclave_u_fstatat64_ocall,
		(void*)Enclave_sgx_oc_cpuidex,
		(void*)Enclave_sgx_thread_wait_untrusted_event_ocall,
		(void*)Enclave_sgx_thread_set_untrusted_event_ocall,
		(void*)Enclave_sgx_thread_setwait_untrusted_events_ocall,
		(void*)Enclave_sgx_thread_set_multiple_untrusted_events_ocall,
		(void*)Enclave_u_sgxprotectedfs_exclusive_file_open,
		(void*)Enclave_u_sgxprotectedfs_check_if_file_exists,
		(void*)Enclave_u_sgxprotectedfs_fread_node,
		(void*)Enclave_u_sgxprotectedfs_fwrite_node,
		(void*)Enclave_u_sgxprotectedfs_fclose,
		(void*)Enclave_u_sgxprotectedfs_fflush,
		(void*)Enclave_u_sgxprotectedfs_remove,
		(void*)Enclave_u_sgxprotectedfs_recovery_file_open,
		(void*)Enclave_u_sgxprotectedfs_fwrite_recovery_node,
		(void*)Enclave_u_sgxprotectedfs_do_file_recovery,
	}
};
sgx_status_t ecall_file_open(sgx_enclave_id_t eid, size_t* retval, int32_t* error, const char* path, uint8_t create, uint8_t protect_integrity, const sgx_key_128bit_t* key)
{
	sgx_status_t status;
	ms_ecall_file_open_t ms;
	ms.ms_error = error;
	ms.ms_path = path;
	ms.ms_path_len = path ? strlen(path) + 1 : 0;
	ms.ms_create = create;
	ms.ms_protect_integrity = protect_integrity;
	ms.ms_key = key;
	status = sgx_ecall(eid, 0, &ocall_table_Enclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t ecall_file_close(sgx_enclave_id_t eid, int* retval, size_t file)
{
	sgx_status_t status;
	ms_ecall_file_close_t ms;
	ms.ms_file = file;
	status = sgx_ecall(eid, 1, &ocall_table_Enclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t ecall_file_flush(sgx_enclave_id_t eid, int* retval, size_t file)
{
	sgx_status_t status;
	ms_ecall_file_flush_t ms;
	ms.ms_file = file;
	status = sgx_ecall(eid, 2, &ocall_table_Enclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t ecall_file_read_at(sgx_enclave_id_t eid, size_t* retval, size_t file, size_t offset, uint8_t* buf, size_t len)
{
	sgx_status_t status;
	ms_ecall_file_read_at_t ms;
	ms.ms_file = file;
	ms.ms_offset = offset;
	ms.ms_buf = buf;
	ms.ms_len = len;
	status = sgx_ecall(eid, 3, &ocall_table_Enclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t ecall_file_write_at(sgx_enclave_id_t eid, size_t* retval, size_t file, size_t offset, const uint8_t* buf, size_t len)
{
	sgx_status_t status;
	ms_ecall_file_write_at_t ms;
	ms.ms_file = file;
	ms.ms_offset = offset;
	ms.ms_buf = buf;
	ms.ms_len = len;
	status = sgx_ecall(eid, 4, &ocall_table_Enclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t ecall_file_get_mac(sgx_enclave_id_t eid, int* retval, size_t file, uint8_t* buf, size_t len)
{
	sgx_status_t status;
	ms_ecall_file_get_mac_t ms;
	ms.ms_file = file;
	ms.ms_buf = buf;
	ms.ms_len = len;
	status = sgx_ecall(eid, 5, &ocall_table_Enclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

