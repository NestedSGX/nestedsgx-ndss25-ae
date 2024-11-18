## Notes on running redis
First, run `setup.sh` to get the necessary preparation done.

Then, on different side, you should run the following scripts.

- in `server`, run all the scripts with prefix `SGX_MODE=SIM`, e.g.
```
SGX_MODE=SIM ./prepare.sh
SGX_MODE=SIM ./build.sh
SGX_MODE=SIM ./run.sh
```
You can run `./server.sh` as a simple way, which contains the commands above.

- In another terminal, in `client`, run `benchmark.sh` with prefix `YCSB_ROOT=../tools/ycsb-redis-binding-0.17.0`
```
YCSB_ROOT=../tools/ycsb-redis-binding-0.17.0 ./benchmark.sh <tag>
```

The result will shows in the `client`. You should fetch and record the result `.csv` file. This will be used in plotting process. You can run client.sh for simplicity.

## Simpler Steps
First run `./setup.sh`

Then run `./redis.sh $outputs` will be OK.
## Ploting
See `redis.py` for more details. notice: for -nsgx or -sim, add two `.csv` files, one is `throughoutput.csv`, the other is `latency.csv`