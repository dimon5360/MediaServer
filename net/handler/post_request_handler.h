
#ifndef NET_SET_REQUEST_HANDLER_H
#define NET_SET_REQUEST_HANDLER_H

#include "irequest_handler.h"

namespace Net::Handler {

class PostRequest : public IRequest {

    friend class RequestWrapper;

    using IRequest::IRequest;

public:

    PostRequest(handler custom_handler) 
        : IRequest() {

        this->handle = custom_handler;

        spdlog::info("PostRequest class constructor");
    }
    
    ~PostRequest() {
        spdlog::info("PostRequest class destructor");
    }

    handler handle = {};
};

}
#endif // NET_SET_REQUEST_HANDLER_H