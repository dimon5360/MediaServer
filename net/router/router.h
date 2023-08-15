
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

	template<boost::beast::http::verb method>
    void setup_route(const std::string& api, Handler::IRequest::handler handle) {
        routing_.insert({ api, Handler::RequestWrapper::wrap(method, handle) });
    }

	std::shared_ptr<Handler::RequestWrapper> operator[](const std::string& api) {
		if (!routing_.contains(api)) {
			return nullptr;
		}

		return routing_.at(api);
	}

private:

    Router() {
        spdlog::info("Router class constructor");
    }

private:

    std::map<std::string, std::shared_ptr<Handler::RequestWrapper>> routing_;
};

}

#endif // NET_ROUTER_H