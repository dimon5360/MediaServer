
#ifndef NET_INTERFACE_REQUEST_HANDLER_H
#define NET_INTERFACE_REQUEST_HANDLER_H

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

#include <string>
#include <spdlog/spdlog.h>
#include <boost/format.hpp>

namespace Net::Handler {

namespace beast = boost::beast;
namespace http = beast::http;

class IRequest {

public:

    using msg_gen = Handler::http::message_generator;
    using req_handler = Handler::http::request<Handler::http::string_body>;
    using handler = std::function<msg_gen(req_handler&& req)>;

    IRequest() noexcept {
        spdlog::info("IRequest class constructor");
    }

    virtual ~IRequest() {
        spdlog::info("IRequest class destructor");
    }

    template<typename T, class Body = http::string_body,
        class Allocator = std::allocator<char>,
        class Request = http::request<Body, http::basic_fields<Allocator>>>
    static http::response<Body> wrong_request(http::status status, T&& fmt, Request&& req) {
        http::response<http::string_body> res{ status, req.version() };
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(std::forward<Request>(req).keep_alive());
        res.body() = boost::str(boost::format(std::forward<T>(fmt)));
        res.prepare_payload();
        return res;
    }
};

}

#endif // NET_INTERFACE_REQUEST_HANDLER_H