
#ifndef NET_GET_REQUEST_HANDLER_H
#define NET_GET_REQUEST_HANDLER_H

#include "irequest_handler.h"

namespace Net::Handler {

class GetRequest : public IRequest {

    using IRequest::IRequest;

public:

    explicit GetRequest(const handler& custom_handler) 
        : IRequest()
        , handle(custom_handler)
    {
        spdlog::info("GetRequest class constructor");
    }

    GetRequest(const GetRequest&) = delete;
    GetRequest(GetRequest&&) = delete;
    const GetRequest& operator=(const GetRequest&) = delete;
    GetRequest&& operator=(GetRequest&&) = delete;

    ~GetRequest() override {
        spdlog::info("GetRequest class destructor");
    }

    const handler& get_handler() const {
        return handle;
    }

private:

    handler handle = {};
};

}

#endif // NET_GET_REQUEST_HANDLER_H