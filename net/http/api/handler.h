
#ifndef NET_API_REST_HANDLER_H
#define NET_API_REST_HANDLER_H

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

#include <string>
#include <spdlog/spdlog.h>
#include <boost/format.hpp>

namespace Net::Http::Api {

namespace beast = boost::beast;
namespace http = beast::http;

using msg_gen = http::message_generator;
using req_handler = http::request<http::string_body>;
using handler = std::function<msg_gen(req_handler&& req)>;

template<typename T, class Body = http::string_body,
    class Allocator = std::allocator<char>,
    class Request = http::request<Body, http::basic_fields<Allocator>>>
inline http::response<Body> wrong_request(http::status status, T&& fmt, Request&& req) {
    http::response<http::string_body> res{ status, req.version() };
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "text/html");
    res.keep_alive(std::forward<Request>(req).keep_alive());
    res.body() = boost::str(boost::format(std::forward<T>(fmt)));
    res.prepare_payload();
    return res;
}

msg_gen h_get_16x16_icon(http::request<http::string_body>&& request_);
msg_gen h_get_32x32_icon(http::request<http::string_body>&& request_);

msg_gen h_get_index_html(http::request<http::string_body>&& request_);

msg_gen h_get_auth_html(http::request<http::string_body>&& request_);
msg_gen h_get_auth_js(http::request<http::string_body>&& request_);
msg_gen h_post_auth_js(http::request<http::string_body>&& request_);

msg_gen h_get_register_html(http::request<http::string_body>&& request_);
msg_gen h_get_register_js(http::request<http::string_body>&& request_);
msg_gen h_post_register_js(http::request<http::string_body>&& request_);
}

#endif // NET_API_REST_HANDLER_H