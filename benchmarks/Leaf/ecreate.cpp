// tests based on simple instruction of intel SGX.
// ECREATE => create on the manager, remove it when the Enclave manager is full.
// two steps are involved.
#include "basic.h"
#include <time.h>
#include <chrono>
#include <string>

#include <stdio.h>
#include <stdlib.h>



// Define the SgxSecs struct
struct SgxSecs {
    uint64_t size;
    uint64_t base_addr;
    uint32_t ssa_frame_size;
    uint32_t misc_select;
    uint8_t _reserved1[16];
    uint64_t ms_buf_size;
    uint64_t test[2];
    uint32_t mr_enclave[8];
    uint8_t _reserved2[32];
    uint32_t mr_signer[8];
    uint8_t _reserved3[32];
    uint32_t config_id[16];
    uint16_t isv_prod_id;
    uint16_t isv_svn;
    uint16_t config_svn;
    uint8_t _reserved64[3834];
};

// Function to generate a random SgxSecs struct
struct SgxSecs generate_random_sgx_secs() {
    struct SgxSecs random_sgx_secs;

    // Initialize each field with random values (you can customize this part)
    random_sgx_secs.size = rand() % 10000 + 8000; // Random size between 8000 and 18000

    return random_sgx_secs;
}

int Ioctl(int fd, int SVSM_IOCTL_REQUEST, struct svsm_service_request *service)
{
    if (ioctl(fd, SVSM_IOCTL_REQUEST, (unsigned long) service) == -1) 
    {
        perror("User App: IOCTL failed\n");
        printf("SVSM_IOCTL_REQUEST: %d\n", SVSM_IOCTL_REQUEST);
        close(fd);
        return -1;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    unsigned long cycles_low, cycles_high, cycles_low1, cycles_high1, cycles_low2, cycles_high2;
    int fd_test = open(DEVICE_PATH, O_RDWR);
    struct svsm_service_request service;
    struct SgxSecs test = generate_random_sgx_secs();
    __asm__ __volatile__("rdtscp" : "=a" (cycles_low), "=d" (cycles_high));
	for (int i = 0; i < TEST_TIME; i++) {
		service.rax = SVSM_FUNCTION(SVSM_SGX_PROTOCOL, SVSM_SGX_ECREATE);
        service.rcx = (unsigned long long) &(test);
        Ioctl(fd_test, SVSM_ECREATE, &service);
	}
	__asm__ __volatile__("rdtscp" : "=a" (cycles_low1), "=d" (cycles_high1));
	unsigned long long start_cycles = ((unsigned long long)(cycles_high) << 32) | (unsigned long long) cycles_low;
	unsigned long long end_cycles = ((unsigned long long)(cycles_high1) << 32) | (unsigned long long) cycles_low1;
	unsigned long long elapsed_cycles = end_cycles - start_cycles;
	printf("elapsed_cycles: %lld per loop: %lld\n", elapsed_cycles, elapsed_cycles / TEST_TIME);
	
	// Start measuring time	
	auto start = std::chrono::steady_clock::now();
	for (int i = 0; i < TEST_TIME; i++) {
	    service.rax = SVSM_FUNCTION(SVSM_SGX_PROTOCOL, SVSM_SGX_ECREATE);
        service.rcx = (unsigned long long) &(test);
        Ioctl(fd_test, SVSM_EADD, &service);
	}
	// Stop measuring time and calculate the elapsed time
	auto end = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	printf("Time measured: %lf u seconds.\n", elapsed.count() * 1e-3 / TEST_TIME);
	
    return 0;
}

