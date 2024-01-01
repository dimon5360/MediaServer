
#ifndef NET_INTERFACE_SERVER_H
#define NET_INTERFACE_SERVER_H

#include <spdlog/spdlog.h>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

namespace Net {

class IConnection {

public:

    IConnection() {
        spdlog::info("IConnection class constructor");
    }

    IConnection(const IConnection&) = delete;
    IConnection(IConnection&&) = delete;

    virtual ~IConnection() {
        spdlog::info("IConnection class destructor");
    }
};
}

#endif // NET_INTERFACE_SERVER_H