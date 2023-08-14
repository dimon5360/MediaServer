
#ifndef NET_SET_REQUEST_HANDLER_H
#define NET_SET_REQUEST_HANDLER_H

#include "irequest_handler.h"

namespace Net::Handler {

class PostRequest : public IRequest {

    friend class Router;

    using IRequest::IRequest;

    using handler = std::function<Handler::http::message_generator(Handler::http::request<Handler::http::string_body>&& req, const std::string& doc_root)>;

public:

    explicit PostRequest(handler custom_handler) 
        : IRequest() {

        this->handler_ = custom_handler;

        spdlog::info("PostRequest class constructor");
    }
    
    ~PostRequest() {
        spdlog::info("PostRequest class destructor");
    }

    http::message_generator execute(http::request<http::string_body>&& request_, const std::string& doc_root_) 
    {
        return handler_(std::move(request_), doc_root_);
    }

    handler handler_ = {};
};

}
#endif // NET_SET_REQUEST_HANDLER_H