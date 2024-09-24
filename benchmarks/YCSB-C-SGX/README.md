# YCSB-C-SGX

SQLite (v3.19.3) database inside Intel SGX enclave, integrated with a C++ version of [Yahoo! Cloud Serving Benchmark](https://github.com/brianfrankcooper/YCSB/wiki).

Reference: 

1. https://github.com/yerzhan7/SGX_SQLite
2. https://github.com/basicthinker/YCSB-C

## Build for native

```
make native
```

## Build for SGX

```
make clean
make SGX_MODE=HW SGX_DEBUG=0 SGX_PRERELEASE=1
```

## Run

```
./app -db sqlite -P YCSB-C/workloads/workloada.spec
```

## Benchmark

Benchmark the transaction throughput using the YCSB workloads as the number of records increase from 5K to 200K.

```
./benchmark.sh [OPTIONS] <TAG>

Options:
    -h                      Display this message
    -r <NUMBER>             Number of runs (default: 10)
    -w <WORKLOAD>           Workload ID in A, B, C, D, E, F (default: A)
    -t <OP_COUNT>           Number of operations (default: 100000)

Arguments:
    <TAG>                   Tag of this evalutaion
```
