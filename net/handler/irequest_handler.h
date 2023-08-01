
#ifndef NET_INTERFACE_REQUEST_HANDLER_H
#define NET_INTERFACE_REQUEST_HANDLER_H

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

#include <string>

namespace Net::Handler {

namespace beast = boost::beast;
namespace http = beast::http;

class IRequest {

public:

    IRequest() noexcept;
    virtual ~IRequest();

    virtual http::message_generator handle(http::request<http::string_body>&&, const std::string&);
    virtual std::string path_cat(beast::string_view, beast::string_view);
};

}

#endif // NET_INTERFACE_REQUEST_HANDLER_H