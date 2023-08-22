
#ifndef NET_ROUTER_H
#define NET_ROUTER_H

#include "handlers.h"

namespace Net {

class Router {

public:

    static Router& instance() {
        static std::shared_ptr<Router> router(new Router());
        return *router;
    }

    Router(const Router&) = delete;
    Router(Router&&) = delete;
    const Router& operator=(const Router&) = delete;
    Router&& operator=(Router&&) = delete;

    ~Router() {
        spdlog::info("Router class destructor");
    }

    void init_routing();

    std::shared_ptr<const Handler::RequestWrapper> operator[](const std::string& api) {
        return routing_.at(api);
    }

private:

    template<boost::beast::http::verb method>
    void add_routing(const std::string& api, const Handler::IRequest::handler& handle) {
        routing_.try_emplace(api, Handler::RequestWrapper::wrap(method, handle));
    }

    explicit Router() {
        spdlog::info("Router class constructor");
    }

private:

    std::map<std::string, std::shared_ptr<Handler::RequestWrapper>, std::less<>> routing_;
};

}

#endif // NET_ROUTER_H