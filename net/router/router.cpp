
#include "router.h"
#include "config.h"
#include "session.h"

#include <string>
#include <future>
#include <thread>

#include <spdlog/spdlog.h>
#include <boost/bind.hpp>
#include <boost/beast.hpp>

#include <boost/json.hpp>
#include <boost/json/src.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include <boost/multiprecision/cpp_int.hpp>

namespace Net {

using namespace boost::asio;
using method = boost::beast::http::verb;

namespace {

const std::string favicon_16x16{ "../static/favicon-16x16.png" };
const std::string favicon_32x32{ "../static/favicon-32x32.png" };

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

static Handler::IRequest::msg_gen h_get_16x16_icon(http::request<http::string_body>&& request_);
static Handler::IRequest::msg_gen h_get_32x32_icon(http::request<http::string_body>&& request_);

static Handler::IRequest::msg_gen h_get_index(http::request<http::string_body>&& request_);
static Handler::IRequest::msg_gen h_get_main(http::request<http::string_body>&& request_);
static Handler::IRequest::msg_gen h_get_login(http::request<http::string_body>&& request_);

static Handler::IRequest::msg_gen h_post_login(http::request<http::string_body>&& request_);

void Router::init_routing() {

    using enum boost::beast::http::verb;

    add_routing<get>("/favicon-16x16.png", h_get_16x16_icon);
    add_routing<get>("/favicon-32x32.png", h_get_32x32_icon);

    add_routing<get>("", h_get_index);
    add_routing<get>("/api/v1/main.html", h_get_main);
    add_routing<get>("/api/v1/login.js", h_get_login);

    add_routing<post>("/api/v1/login.js", h_post_login);
}

static Handler::IRequest::msg_gen prepare_response_with_file(http::request<http::string_body>&& request_, const std::string& filename, const std::string& content_type) {

    beast::error_code ec;
    http::file_body::value_type body;

    body.open(filename.c_str(), beast::file_mode::scan, ec);

    if (beast::errc::no_such_file_or_directory == ec)
        return Handler::IRequest::wrong_request(wrongFile(request_.target()), std::move(request_));

    if (beast::errc::success != ec)
        return Handler::IRequest::wrong_request(errorOccured(ec.message()), std::move(request_));

    auto const size = body.size();

    auto header{ std::make_tuple(http::status::ok, request_.version()) };
    http::response<http::file_body> res{ std::piecewise_construct, std::make_tuple(std::move(body)), header };
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, content_type);
    res.content_length(size);
    res.keep_alive(request_.keep_alive());
    return res;
}


static Handler::IRequest::msg_gen h_get_index(http::request<http::string_body>&& request_) {

    spdlog::info("called {}", std::source_location::current().function_name());

    return prepare_response_with_file(std::move(request_), index_html, "text/html");
}

static Handler::IRequest::msg_gen h_get_login(http::request<http::string_body>&& request_) {

    spdlog::info("called {}", std::source_location::current().function_name());

    return prepare_response_with_file(std::move(request_), login_js, "text/javascript");
}

static bool validateCreadentials(std::string&& login, std::string&& password) {

    // TODO: check credentials with database
    return (0 == std::move(login).compare("admin")) && (0 == std::move(password).compare("password"));
}

static uint32_t random_number() {

    boost::random::mt19937 gen;
    boost::random::uniform_int_distribution<uint32_t> dist(1, INT32_MAX);
    return dist(gen);
}

static Handler::IRequest::msg_gen h_post_login(http::request<http::string_body>&& request_) {

    namespace pt = boost::property_tree;

    spdlog::info("called {}", std::source_location::current().function_name());

    beast::error_code ec;
    boost::json::value req_json = boost::json::parse(request_.body(), ec);
    std::string resp_body{};

    try {
        std::string login{ req_json.at("username").as_string() };
        std::string password{ req_json.at("password").as_string() };

        // async login data validation
        decltype(auto) fut = std::async([&login, &password]() {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            return validateCreadentials(std::move(login), std::move(password));
        });

        if (!fut.get()) {
            return Handler::IRequest::wrong_request(errorOccured("Invalid login or password"), std::move(request_));
        }

        pt::ptree resp_json;
        resp_json.put("uuid", random_number());

        std::ostringstream oss;
        boost::property_tree::json_parser::write_json(oss, resp_json);
        resp_body = oss.str();

        spdlog::info("json response: {}", resp_body);
    }
    catch (const std::out_of_range& ex) {
        spdlog::error("The exception caught: {}", ex.what());
    }

    auto const size = resp_body.length();
    http::response<http::string_body> res{ http::status::ok, request_.version() };
    res.set(http::field::content_type, "application/json");
    res.keep_alive(request_.keep_alive());
    res.content_length(size);
    res.body() = resp_body;
    res.prepare_payload();
    return res;
}

static Handler::IRequest::msg_gen h_get_main(http::request<http::string_body>&& request_) {

    spdlog::info("called {}", std::source_location::current().function_name());

    return prepare_response_with_file(std::move(request_), main_html, "text/html");
}

static Handler::IRequest::msg_gen h_get_16x16_icon(http::request<http::string_body>&& request_) {

    spdlog::info("called {}", std::source_location::current().function_name());

    return prepare_response_with_file(std::move(request_), favicon_16x16, "image/png");
}

static Handler::IRequest::msg_gen h_get_32x32_icon(http::request<http::string_body>&& request_) {

    spdlog::info("called {}", std::source_location::current().function_name());

    return prepare_response_with_file(std::move(request_), favicon_32x32, "image/png");
}
}