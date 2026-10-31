# jfc-net_enet
The [jfc-net](https://github.com/jfcameron/jfc-net) transport interface, implemented over UDP using [ENet](https://github.com/lsalzman/enet).

API documentation: https://jfcameron.github.io/jfc-net_enet/

## Getting Started
Install cmake 3.21 or greater, and a c++20 compatible compiler
```
# initialize the tree
git submodule update --init --recursive

# generate the script to build the library and its tests on linux using gcc
cmake --preset linux-gcc

# build the library and tests
cmake --build --preset linux-gcc

# run the tests
ctest --preset linux-gcc
```
See `CMakePresets.json` for the full list of supported platforms and compilers. See `.github/workflows/ci.yml` to see exactly how CI runners build it on different platforms.

To use it from another project, add it with `add_subdirectory` and link `jfcnet_enet`. ENet is linked privately, so its headers never reach the project using it. The installed package is `jfc-net_enet`, target `jfc::net_enet`. A project that has already added ENet or jfc-net gets one copy of each.

## Overview
### `enet_host`
A transport between processes over UDP. `listen` makes a server's end on a port, and `connect` makes a client's end. Reliable and unreliable messages each travel on an ENet channel of their own, and a peer that goes silent is let go after a timeout, 5 seconds by default. Polling is what services the connection, so a host must be polled regularly to stay connected, and a peer should only be sent to after its connected event, since anything sent before is dropped. `lan_address` gives the address other machines on the local network would use to reach this one, for a hosting screen to show.

It is IPv4 only. Browsers do not allow UDP, so this library refuses to configure under Emscripten: pages use [jfc-net_webrtc](https://github.com/jfcameron/jfc-net_webrtc).
