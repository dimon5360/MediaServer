
#include "irequest_handler.h"

#include <spdlog/spdlog.h>
#include <boost/format.hpp>

namespace Net::Handler {

IRequest::IRequest() noexcept {
    spdlog::info("IRequest class constructor");
}

IRequest::~IRequest() {
    spdlog::info("IRequest class destructor");
}

template<typename T, class Body = http::string_body, class Allocator = std::allocator<char>>
http::response<Body> wrong_request(T&& fmt, http::request<Body, http::basic_fields<Allocator>>& req) {
    http::response<http::string_body> res{ http::status::bad_request, req.version() };
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "text/html");
    res.keep_alive(req.keep_alive());
    res.body() = boost::str(boost::format(fmt));
    res.prepare_payload();
    return res;
}

http::message_generator IRequest::handle(http::request<http::string_body>&& req, const std::string& doc_root) {
    return wrong_request(std::string("Unknown HTTP-method"), req);
}

std::string IRequest::path_cat(beast::string_view base, beast::string_view path) {
    if (base.empty())
        return std::string(path);
    std::string result(base);
    char constexpr path_separator = '/';
    if (result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
    return result;
}
}