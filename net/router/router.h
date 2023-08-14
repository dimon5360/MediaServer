
#ifndef NET_ROUTER_H
#define NET_ROUTER_H

#include "handler.h"

#include <string>
#include <memory>
#include <map>
#include <utility>
#include <functional>

namespace Net {

class Router {

private:

    using api = std::string;
    using method = boost::beast::http::verb;

public:

    using handler = std::function<Handler::http::message_generator(Handler::http::request<Handler::http::string_body>&& req, const std::string& doc_root)>;

    static std::shared_ptr<Router> instance() {
        static std::shared_ptr<Router> router(new Router());
        return router;
    }

    Router(const Router&) = delete;
    Router(Router&&) = delete;
    const Router& operator=(const Router&) = delete;
    Router&& operator=(Router&&) = delete;

    ~Router() {
        spdlog::info("Router class destructor");
    }

    void setup_route(const api& api_, std::shared_ptr<Net::Handler::IRequest> handler) {
        routing_.insert({api_, handler});
    }

    Handler::http::message_generator execute(Handler::http::request<Handler::http::string_body>&& request_, const std::string& doc_root_) {

        auto method = request_.method();
        auto path = request_.target();

        if (!routing_.contains(path)) {
            return Handler::IRequest::wrong_request(std::string("Illegal request-target"), request_);
        }

        switch(request_.method())
        {
            case Handler::http::verb::get: {
                spdlog::info("Received GET request with API {}", path);
                auto h = static_cast<Handler::GetRequest*>(routing_.at(path).get());
                return h->execute(std::move(request_), doc_root_);
            }

            case Handler::http::verb::post: {
                spdlog::info("Received POST request with API {}", path);
                auto h = static_cast<Handler::PostRequest*>(routing_.at(path).get());
                return h->execute(std::move(request_), doc_root_);
            }

            default:
            {
                return Handler::IRequest::wrong_request(std::string("Unknown HTTP-method"), request_);
            }
        } 
    }

private:

    Router() {
        spdlog::info("Router class constructor");
    }

private:

    std::map<api, std::shared_ptr<Net::Handler::IRequest>> routing_;
};

}

#endif // NET_ROUTER_H