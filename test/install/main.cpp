// © Joseph Cameron - MIT License

#include <jfc/net/enet_host.h>

int main() { return jfc::net::enet_host::listen(0)->port() ? 0 : 1; }
