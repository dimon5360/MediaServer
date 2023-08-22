
#include "router.h"
#include "config.h"
#include "session.h"

#include <string>
#include <spdlog/spdlog.h>
#include <boost/bind.hpp>
#include <boost/beast.hpp>
#include <boost/json.hpp>
#include <boost/json/src.hpp>

namespace Net {
using namespace boost::asio;
using method = boost::beast::http::verb;

namespace {

const std::string index_html{ "../static/html/index.html" };
const std::string files_list_html{ "../static/html/files_list.html" };

const auto wrongFile = [](std::string_view resource) -> decltype(auto) {
    return boost::str(boost::format("The resourse '%1%' was not found.") % resource);
};

const auto errorOccured = [](std::string_view err_msg) -> decltype(auto) {
    return boost::str(boost::format("An error occurred: '%1%'") % err_msg);
};
}

static Handler::IRequest::msg_gen h_get_index(http::request<http::string_body>&& request_);
static Handler::IRequest::msg_gen h_get_list(http::request<http::string_body>&& request_);

void Router::init_routing() {

    add_routing<boost::beast::http::verb::get>("/", h_get_index);
    add_routing<boost::beast::http::verb::get>("/api/v1/get/list/", h_get_list);
}

static Handler::IRequest::msg_gen h_get_index(http::request<http::string_body>&& request_) {
    beast::error_code ec;
    http::file_body::value_type body;
    body.open(index_html.c_str(), beast::file_mode::scan, ec);

    if (beast::errc::no_such_file_or_directory == ec)
        return Handler::IRequest::wrong_request(wrongFile(request_.target()), std::move(request_));

    if (beast::errc::success != ec)
        return Handler::IRequest::wrong_request(errorOccured(ec.message()), std::move(request_));

    auto const size = body.size();

    auto header{ std::make_tuple(http::status::ok, request_.version()) };
    http::response<http::file_body> res{ std::piecewise_construct, std::make_tuple(std::move(body)), header };
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "text/html");
    res.content_length(size);
    res.keep_alive(request_.keep_alive());
    return res;
}

static Handler::IRequest::msg_gen h_get_list(http::request<http::string_body>&& request_) {

    beast::error_code ec;
    http::file_body::value_type body;
    body.open(files_list_html.c_str(), beast::file_mode::scan, ec);

    if (beast::errc::no_such_file_or_directory == ec)
        return Handler::IRequest::wrong_request(wrongFile(request_.target()), std::move(request_));

    if (beast::errc::success != ec)
        return Handler::IRequest::wrong_request(errorOccured(ec.message()), std::move(request_));

    auto const size = body.size();

    auto header{ std::make_tuple(http::status::ok, request_.version()) };
    http::response<http::file_body> res{ std::piecewise_construct, std::make_tuple(std::move(body)), header };
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "text/html");
    res.content_length(size);
    res.keep_alive(request_.keep_alive());
    return res;
}


static Handler::IRequest::msg_gen test_json_response(http::request<http::string_body>&& request_) {
    const std::string json_str = R"(
        {
            "key": "1234",
            "value": "Hello, world!"
        }
    )";

    http::string_body::value_type body(json_str.c_str());

    const auto size = body.size();
    auto header{ std::make_tuple(http::status::ok, request_.version()) };
    http::response<http::string_body> res{ std::piecewise_construct, std::make_tuple(std::move(body)), header };
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "application/json");
    res.content_length(size);
    res.keep_alive(request_.keep_alive());
    return res;
}
}