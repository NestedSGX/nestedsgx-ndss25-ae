## Leaf instruction
Test for the latency for SGX Leaf Instruction.

This is not a complete set, for NestedSGX, we only Test the needed Leaf instructions. And Instructions like EENTER/EEXIT are also excluded.
### Provide Leaf Function benchmarks
| Inst | number | Implement |
| ------   | -- | --- |
| ECREATE: | 1 | &#10004; |
| EADD: | 2 | &#10004; |
| EEXTEND: | 5 | &#10004;|
| EINIT: | 3 | |

### Others:
scp -r link@192.168.104.4:/home/link/nestedsgx_archive/benchmarks/Leaf .