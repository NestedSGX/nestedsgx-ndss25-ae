## Notes on running tls:
- first in `SGX-OpenSSL`, setup the OpenSSL_SGX as the `README.md` describes.
- second, in `SampleCode`, build the `Simple_TLS_Server` in Simulation mode, run it with `./app`
- Now in `tls-perf`, run `./tls-perf`. remember the [ip port] value by default is `127.0.0.1 4433`.

Although `TLS` can operate well in `NestedSGX`, it might trigger some bugs and will be buggy when the program ends. If the KVM crashes, please reboot the guest OS as you obtain the handshake times.