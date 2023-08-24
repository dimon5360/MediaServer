
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
const std::string main_html{ "../static/html/main.html" };

const std::string login_js{ "../static/js/login.js" };

const auto wrongFile = [](std::string_view resource) -> decltype(auto) {
    return boost::str(boost::format("The resourse '%1%' was not found.") % resource);
};

const auto errorOccured = [](std::string_view err_msg) -> decltype(auto) {
    return boost::str(boost::format("An error occurred: '%1%'") % err_msg);
};
}

static Handler::IRequest::msg_gen h_get_index(http::request<http::string_body>&& request_);
static Handler::IRequest::msg_gen h_get_main(http::request<http::string_body>&& request_);
static Handler::IRequest::msg_gen h_get_login(http::request<http::string_body>&& request_);

static Handler::IRequest::msg_gen h_post_login(http::request<http::string_body>&& request_);

void Router::init_routing() {

    using enum boost::beast::http::verb;

    add_routing<get>("", h_get_index);
    add_routing<get>("/api/v1/main", h_get_main);
    add_routing<get>("/api/v1/login.js", h_get_login);

    add_routing<post>("/api/v1/login.js", h_post_login);
}

static Handler::IRequest::msg_gen h_get_index(http::request<http::string_body>&& request_) {

    spdlog::info("called {}", std::source_location::current().function_name());

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

static Handler::IRequest::msg_gen h_get_login(http::request<http::string_body>&& request_) {

    spdlog::info("called {}", std::source_location::current().function_name());

    beast::error_code ec;
    http::file_body::value_type body;

    body.open(login_js.c_str(), beast::file_mode::scan, ec);

    if (beast::errc::no_such_file_or_directory == ec)
        return Handler::IRequest::wrong_request(wrongFile(request_.target()), std::move(request_));

    if (beast::errc::success != ec)
        return Handler::IRequest::wrong_request(errorOccured(ec.message()), std::move(request_));

    const auto size = body.size();

    auto header{ std::make_tuple(http::status::ok, request_.version()) };
    http::response<http::file_body> res{ std::piecewise_construct, std::make_tuple(std::move(body)), header };
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "text/javascript");
    res.content_length(size);
    res.keep_alive(request_.keep_alive());
    return res;
}

static bool validateCreadentials(std::string&& login, std::string&& password) {

    // TODO: check credentials with database
    return (0 == login.compare("admin")) && (0 == password.compare("password"));
}

static Handler::IRequest::msg_gen h_post_login(http::request<http::string_body>&& request_) {

    spdlog::info("called {}", std::source_location::current().function_name());

    beast::error_code ec;
    boost::json::value json = boost::json::parse(request_.body(), ec);
    std::string resp_body;

    try {
        std::string login{ json.at("username").as_string() };
        std::string password{ json.at("password").as_string() };

        if (!validateCreadentials(std::move(login), std::move(password))) {
            return Handler::IRequest::wrong_request(errorOccured("Invalid login or password"), std::move(request_));
        }

        resp_body = { boost::str(boost::format("Hello, %1%") % login) };
    }
    catch (const std::out_of_range& ex) {
        spdlog::error("The exception caught: {}", ex.what());
    }

    auto header{ std::make_tuple(http::status::ok, request_.version()) };
    http::response<http::string_body> res{ std::piecewise_construct, std::make_tuple(std::move(resp_body)), header };;
    res.result(http::status::ok);
    res.version(request_.version());
    res.set(http::field::content_type, "application/json");
    res.keep_alive(request_.keep_alive());
    res.content_length(resp_body.length());
    res.keep_alive(request_.keep_alive());
    return res;
}

static Handler::IRequest::msg_gen h_get_main(http::request<http::string_body>&& request_) {

    spdlog::info("called {}", std::source_location::current().function_name());

    beast::error_code ec;
    http::file_body::value_type body;
    body.open(main_html.c_str(), beast::file_mode::scan, ec);

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
}