
#ifndef NET_INTERFACE_SERVER_H
#define NET_INTERFACE_SERVER_H

#include <spdlog/spdlog.h>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

namespace Net {

class IServer {

public:

    IServer() {
        spdlog::info("Iserver class constructor");
    }

    IServer(const IServer&) = delete;
    IServer(IServer&&) = delete;

    virtual ~IServer() {
        spdlog::info("Iserver class destructor");
    }
};
}

#endif // NET_INTERFACE_SERVER_H