
#include "handler.h"

#include <spdlog/spdlog.h>
#include <boost/format.hpp>

namespace Handler {

namespace {

}

Request::Request(http::request<http::string_body>&& req, const std::string& doc_root)
    : request_(std::move(req))
    , doc_root_(doc_root)
{
    spdlog::info("Request class constructor");
}

Request::~Request()
{
    spdlog::info("Request class destructor");
}

template<typename T, class Body = http::string_body, class Allocator = std::allocator<char>>
http::response<Body> wrong_request(T&& fmt, http::request<Body, http::basic_fields<Allocator>>& req) 
{
    http::response<http::string_body> res{http::status::bad_request, req.version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "text/html");
    res.keep_alive(req.keep_alive());
    res.body() = boost::str(boost::format(fmt));
    res.prepare_payload();
    return res;
}

std::string path_cat(beast::string_view base, beast::string_view path)
{
    if(base.empty())
        return std::string(path);
    std::string result(base);
    char constexpr path_separator = '/';
    if(result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
    return result;
}

http::message_generator Request::handle()
{
    if (request_.method() != http::verb::get &&
        request_.method() != http::verb::head) 
    {
        return wrong_request(std::string("Unknown HTTP-method"), request_);
    }

    if (request_.target().empty() ||
        request_.target()[0] == '/' ||
        request_.target().find("..") != beast::string_view::npos)
    {
        return wrong_request(std::string("Illegal request-target"), request_);
    }

    std::string path = path_cat(doc_root_, request_.target());
    if (request_.target().back() == '/') 
        path.append("index.html");

    beast::error_code ec;
    http::file_body::value_type body;
    body.open(path.c_str(), beast::file_mode::scan, ec);

    if (beast::errc::no_such_file_or_directory == ec) 
        return wrong_request(boost::format("The resourse '%1%' was not found.") % request_.target(), request_);

    if (ec) 
        return wrong_request(boost::format("An error occurred: '%1%'") % ec.message(), request_);   

    auto const size = body.size();

    if (request_.method() == http::verb::head) {
        http::response<http::empty_body> res{http::status::ok, request_.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.content_length(size);
        res.keep_alive(request_.keep_alive());
        return res;
    }

    auto header { std::make_tuple(http::status::ok, request_.version()) };
    http::response<http::file_body> res{std::piecewise_construct, std::make_tuple(std::move(body)), header};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "text/html");
    res.content_length(size);
    res.keep_alive(request_.keep_alive());
    return res;
}

}
