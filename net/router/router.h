
#ifndef NET_ROUTER_H
#define NET_ROUTER_H

#include "handler.h"

#include <string>
#include <memory>
#include <map>

namespace Net {

class Router {

public:

    static std::shared_ptr<Router> instance();

    Router(const Router&) = delete;
    Router(Router&&) = delete;
    const Router& operator=(const Router&) = delete;
    Router&& operator=(Router&&) = delete;

    ~Router();

    template<typename T = std::string>
    const std::shared_ptr<Net::Handler::IRequest> operator[](T&& key) const {
        return api_.at(key);
    }

private:

    Router();

    void initRouting();

private:

    std::map<std::string, std::shared_ptr<Net::Handler::IRequest>> api_;
};

}

#endif // NET_ROUTER_H