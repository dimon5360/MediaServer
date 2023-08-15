
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

    using handler = std::function<Handler::http::message_generator(Handler::http::request<Handler::http::string_body>&& req, const std::string& doc_root)>;

    IRequest() noexcept {
        spdlog::info("IRequest class constructor");
    }

    virtual ~IRequest() {
        spdlog::info("IRequest class destructor");
    }

    template<typename T, class Body = http::string_body, class Allocator = std::allocator<char>>
    static http::response<Body> wrong_request(T&& fmt, http::request<Body, http::basic_fields<Allocator>>& req) {
        http::response<http::string_body> res{ http::status::bad_request, req.version() };
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = boost::str(boost::format(fmt));
        res.prepare_payload();
        return res;
    }
};

}

#endif // NET_INTERFACE_REQUEST_HANDLER_H