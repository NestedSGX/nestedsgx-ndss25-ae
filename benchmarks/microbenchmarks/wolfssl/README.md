## To Set up Intel SGX.
Please refer to `wolfssl-examples/SGX_Linux/README.md` to build up your environment.

Two steps:
1. in `v5.7.0-stable`, find `IDE/LINUX-SGX` and build some related static library.
2. in `wolfssl-examples`, build up the `App` binary and run `-b` to get the benchmarks.

Make sure to run in SIM mode.

## Notice:
Make sure you've set up git.username and git.emailaddress beforehand, or the `./build.sh` might not able to apply the patch we provide.

## Plotting:
Ensure you've gotten NSGX.txt and SIM.txt, use analysis.py to transfer it into better form.
```
python analysis.py NSGX.txt > nsgx.txt
```
Then, input them to wolfssl.py
```
python wolfssl.py -nsgx nsgx.txt -sim sim.txt
```