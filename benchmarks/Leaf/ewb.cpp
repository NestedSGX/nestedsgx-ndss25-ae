// tests based on simple instruction of intel SGX.
// EAUG => do memory allocation in svsm for enclaves.
#include "basic.h"
#include <time.h>
#include <chrono>
#include <string>

#include <stdio.h>
#include <stdlib.h>


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
    char array[4096] = {0};
    struct svsm_service_request service;
    __asm__ __volatile__("rdtscp" : "=a" (cycles_low), "=d" (cycles_high));
	for (int i = 0; i < TEST_TIME; i++) {
		service.rax = SVSM_FUNCTION(SVSM_SGX_PROTOCOL, SVSM_SGX_EWB);
        service.rcx = (unsigned long) array;
        Ioctl(fd_test, SVSM_EWB, &service);
	}
	__asm__ __volatile__("rdtscp" : "=a" (cycles_low1), "=d" (cycles_high1));
	unsigned long long start_cycles = ((unsigned long long)(cycles_high) << 32) | (unsigned long long) cycles_low;
	unsigned long long end_cycles = ((unsigned long long)(cycles_high1) << 32) | (unsigned long long) cycles_low1;
	unsigned long long elapsed_cycles = end_cycles - start_cycles;
	printf("elapsed_cycles: %lld per loop: %lld\n", elapsed_cycles, elapsed_cycles / TEST_TIME);
	
	// Start measuring time	
	auto start = std::chrono::steady_clock::now();
	for (int i = 0; i < TEST_TIME; i++) {
		service.rax = SVSM_FUNCTION(SVSM_SGX_PROTOCOL, SVSM_SGX_EWB);
        service.rcx = (unsigned long) array;
        Ioctl(fd_test, SVSM_EWB, &service);
	}
	// Stop measuring time and calculate the elapsed time
	auto end = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	printf("Time measured: %lf u seconds.\n", elapsed.count() * 1e-3 / TEST_TIME);
	
    return 0;
}
