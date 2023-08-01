
#ifndef NET_ROUTER_H
#define NET_ROUTER_H

#include "handler.h"

#include <string>
#include <memory>
#include <map>

namespace Net::Router {

class Router {

public:

    static std::shared_ptr<Router> create();

    Router(const Router&) = delete;
    Router(Router&&) = delete;
    const Router& operator=(const Router&) = delete;
    Router&& operator=(Router&&) = delete;

    ~Router();

private:

    Router();

private:
    std::map<std::string, std::unique_ptr<Net::Handler::IRequest>> api;
};

}

#endif // NET_ROUTER_H