## Notes on running redis
- run `setup.sh` to get the necessary preparation done.
- in `server`, run all the scripts with prefix `SGX_MODE=SIM`, e.g.
```
SGX_MODE=SIM ./prepare.sh
SGX_MODE=SIM ./build.sh
SGX_MODE=SIM ./run.sh
```
- in `client`, run `benchmark.sh` with prefix `YCSB_ROOT=../tools/ycsb-redis-binding-0.17.0`
```
YCSB_ROOT=../tools/ycsb-redis-binding-0.17.0 ./benchmark.sh <tag>
```

The result will shows in the `client`.