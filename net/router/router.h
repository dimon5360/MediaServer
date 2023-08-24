
#ifndef NET_ROUTER_H
#define NET_ROUTER_H

#include "handlers.h"

#include <regex>
#include <algorithm>

namespace Net {

class Router {

    using router_key = std::pair<boost::beast::http::verb, std::string>;

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

    std::shared_ptr<const Handler::RequestWrapper> get_wrapper(boost::beast::http::verb method, std::string_view api) {
        std::string filtered_api{ api };
        if (filtered_api.ends_with('/')) {
            filtered_api.erase(filtered_api.length() - 1);
        }
        return routing_.at(std::make_pair(method, filtered_api));
    }

private:

    template<boost::beast::http::verb method>
    void add_routing(const std::string& api, const Handler::IRequest::handler& handle) {

        std::string filtered_api{ api };
        if (filtered_api.ends_with('/')) {
            filtered_api.erase(api.length() - 1);
        }
        routing_.try_emplace(std::make_pair(method, filtered_api), Handler::RequestWrapper::wrap(method, handle));
    }

    explicit Router() {
        spdlog::info("Router class constructor");
    }

private:

    std::map<router_key, std::shared_ptr<Handler::RequestWrapper>, std::less<>> routing_;
};

}

#endif // NET_ROUTER_H