## Leaf instruction
Test for the latency for SGX Leaf Instruction.

This is not a complete set, for NestedSGX, we only Test the needed Leaf instructions. And Instructions like EENTER/EEXIT are also excluded.
### Provide Leaf Function benchmarks
In fact the benchmarking version of these leaf functions might sound very weird, since the SGX should always do a complete work, therefore simply benchmarking the instructions might not be the accurate answers. 

We set up this benchmarks simply for a evaluation. We will try to keep the main tasks of these instructions unchanged.

| Inst | number | Implement |
| ------   | -- | --- |
| ECREATE: | 1 | &#10004; |
| EADD: | 2 | &#10004; |
| EEXTEND: | 5 | &#10004;|
| EINIT: | 3 | &#10004; |
| EREMOVE: | 6 | &#10004; |


### Others:
scp -r link@192.168.104.4:/home/link/nestedsgx_archive/benchmarks/Leaf .