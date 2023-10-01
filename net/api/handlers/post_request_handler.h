
#ifndef NET_SET_REQUEST_HANDLER_H
#define NET_SET_REQUEST_HANDLER_H

#include "irequest_handler.h"

namespace Net::Handler {

class PostRequest : public IRequest {

    using IRequest::IRequest;

public:

    explicit PostRequest(const handler& custom_handler) 
        : IRequest()
        , handle(custom_handler)
    {
        spdlog::info("PostRequest class constructor");
    }

    PostRequest(const PostRequest&) = delete;
    PostRequest(PostRequest&&) = delete;
    const PostRequest& operator=(const PostRequest&) = delete;
    PostRequest&& operator=(PostRequest&&) = delete;
    
    ~PostRequest() override {
        spdlog::info("PostRequest class destructor");
    }

    const handler& get_handler() const {
        return handle;
    }

private:

    handler handle = {};
};

}
#endif // NET_SET_REQUEST_HANDLER_H