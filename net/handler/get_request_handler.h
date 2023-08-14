
#ifndef NET_GET_REQUEST_HANDLER_H
#define NET_GET_REQUEST_HANDLER_H

#include "irequest_handler.h"

namespace Net::Handler {

class GetRequest : public IRequest {

    using IRequest::IRequest;

    using handler = std::function<Handler::http::message_generator(Handler::http::request<Handler::http::string_body>&& req, const std::string& doc_root)>;

public:

    GetRequest(handler custom_handler) 
        : IRequest() {

        this->handler_ = custom_handler;

        spdlog::info("GetRequest class constructor");
    }

    ~GetRequest() {
        spdlog::info("GetRequest class destructor");
    }

    http::message_generator execute(http::request<http::string_body>&& request_, const std::string& doc_root_) 
    {
        return handler_(std::move(request_), doc_root_);
    }

private:

    handler handler_ = {};
};

}

#endif // NET_GET_REQUEST_HANDLER_H