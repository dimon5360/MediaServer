
#ifndef NET_GET_REQUEST_HANDLER_H
#define NET_GET_REQUEST_HANDLER_H

#include "irequest_handler.h"

namespace Net::Handler {

class GetRequest : public IRequest {

    friend class RequestWrapper;

    using IRequest::IRequest;

public:

    GetRequest(handler custom_handler) 
        : IRequest() {

        this->handle = custom_handler;

        spdlog::info("GetRequest class constructor");
    }

    ~GetRequest() {
        spdlog::info("GetRequest class destructor");
    }

protected:

    handler handle = {};
};

}

#endif // NET_GET_REQUEST_HANDLER_H