## Notes on running tls:
- first in `SGX-OpenSSL`, setup the OpenSSL_SGX as the `README.md` describes.
- second, in `SampleCode`, build the `Simple_TLS_Server` in Simulation mode, run it with `./app`
- Now in `tls-perf`, run `./tls-perf`. remember the [ip port] value by default is `127.0.0.1 4433`.

Although `TLS` can operate well in `NestedSGX`, it might trigger some bugs and will be buggy when the program ends. If the KVM crashes, please reboot the guest OS as you obtain the handshake times.

To set it up, you can simply run `./tls.sh`. However, for `NestedSGX`, we still got some possbilities to crash, so we won't provide scripts to run it automatically, please do it manually.

## Steps
on Server side, run `./app`

on Client side, run `./tls-perf -t <thread numbers> 127.0.0.1 4433`

Record the Handshake times per second manually, and then draw graphs in `tls.py`, please change the value of the following two parameters before drawing the graph.
```
bw_sim_lookup_250
bw_nsgx_lookup_250
```