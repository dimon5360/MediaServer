
#ifndef NET_ROUTER_H
#define NET_ROUTER_H

#include "http/api/handler.h"

#include <regex>
#include <map>
#include <algorithm>

namespace Net {

class Router {

    using router_key = std::pair<boost::beast::http::verb, std::string>;

public:

    static Router& instance() {
        static Router router;
        return router;
    }

    Router(const Router&) = delete;
    Router(Router&&) = delete;
    const Router& operator=(const Router&) = delete;
    Router&& operator=(Router&&) = delete;

    ~Router() {
        spdlog::info("Router class destructor");
    }

    void init_routing();

    Api::Rest::handler& get_handler(boost::beast::http::verb method, std::string_view api) {

        std::string filtered_api{ api };
        if (filtered_api.ends_with('/')) {
            filtered_api.erase(filtered_api.length() - 1);
        }
        return routing_.at(std::make_pair(method, filtered_api));
    }

private:

    template<boost::beast::http::verb method>
    void set_handler(const std::string& api, const Api::Rest::handler& handler) {

        std::string filtered_api{ api };
        if (filtered_api.ends_with('/')) {
            filtered_api.erase(api.length() - 1);
        }
        routing_.try_emplace(std::make_pair(method, filtered_api), handler);
    }

    explicit Router() {
        spdlog::info("Router class constructor");
    }

private:

    std::map<router_key, Api::Rest::handler, std::less<>> routing_;
};

}

#endif // NET_ROUTER_H