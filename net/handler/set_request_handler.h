
#ifndef NET_SET_REQUEST_HANDLER_H
#define NET_SET_REQUEST_HANDLER_H

#include "handler.h"

namespace Net::Handler {

class SetRequest : public IRequest {

    using IRequest::IRequest;

public:

    explicit SetRequest();
    ~SetRequest();

    http::message_generator handle(http::request<http::string_body>&& req, const std::string& doc_root) override;

    std::string path_cat(beast::string_view base, beast::string_view path) override {
        return IRequest::path_cat(base, path);
    }
};

}
#endif // NET_SET_REQUEST_HANDLER_H