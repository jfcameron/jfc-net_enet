// © Joseph Cameron - MIT License

#include "conformance.h"

#include <jfc/net/enet_host.h>

#include <chrono>
#include <thread>

namespace jfc::net::conformance {
    hosts make_hosts(const std::size_t aClients) {
        enet_host_settings settings;
        settings.most_message_bytes = 1024 * 1024;

        std::shared_ptr<enet_host> pServer = enet_host::listen(0, settings);

        hosts out{pServer, {}, settings.most_message_bytes};

        for (std::size_t i = 0; i < aClients; ++i)
            out.clients.push_back(enet_host::connect("127.0.0.1", pServer->port(), settings));

        return out;
    }

    void wait() { std::this_thread::sleep_for(std::chrono::milliseconds(1)); }

    std::string_view implementation() { return "enet"; }
}
