
#include "handler.h"
#include "config.h"

#if 0
#include "grpc_conn.h"
#endif 

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

#include <pqxx/pqxx>

namespace Net::Http::Api {

using method = boost::beast::http::verb;

namespace beast = boost::beast;
namespace http = beast::http;

namespace {

const std::string favicon_16x16{ "./static/icons/favicon-16x16.png" };
const std::string favicon_32x32{ "./static/icons/favicon-32x32.png" };

const std::string index_html{ "./static/html/index.html" };

const std::string auth_html{ "./static/html/auth.html" };
const std::string register_html{ "./static/html/register.html" };

const std::string auth_js{ "./static/js/auth.js" };
const std::string register_js{ "./static/js/register.js" };

const auto wrongFile = [](std::string_view resource) -> decltype(auto) {
    return boost::str(boost::format("The resourse '%1%' was not found.") % resource);
};

const auto errorOccured = [](std::string_view err_msg) -> decltype(auto) {
    return boost::str(boost::format("An error occurred: '%1%'") % err_msg);
};
}

msg_gen prepare_response_with_file(http::request<http::string_body>&& request_, const std::string& filename, const std::string& content_type) {

    beast::error_code ec;
    http::file_body::value_type body;

    body.open(filename.c_str(), beast::file_mode::scan, ec);

    if (beast::errc::no_such_file_or_directory == ec)
        return wrong_request(http::status::bad_request, wrongFile(request_.target()), std::move(request_));

    if (beast::errc::success != ec)
        return wrong_request(http::status::bad_request, errorOccured(ec.message()), std::move(request_));

    auto const size = body.size();

    auto header{ std::make_tuple(http::status::ok, request_.version()) };
    http::response<http::file_body> res{ std::piecewise_construct, std::make_tuple(std::move(body)), header };
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, content_type);
    res.content_length(size);
    res.keep_alive(request_.keep_alive());
    return res;
}

bool validateCreadentials(std::string&& login, std::string&& password) {

    try {
        pqxx::connection c{ App::Config::instance()["CONNECTION_STRING"] };
        pqxx::work txn{ c };

        const std::string query{ "SELECT id FROM users where username=\'" + std::move(login) +
            "\' and password=\'" + std::move(password) + "\';" };

        pqxx::result r{ txn.exec(query) };

        if (r.empty()) {
            // TODO: return false;
            return true;
        }

        for (auto row : r) {
            std::cout << row[0].as<int>() << std::endl;
        }
        txn.commit();

        std::this_thread::sleep_for(std::chrono::seconds(2));
        return true;
    }
    catch (pqxx::sql_error const& e) {
        std::cerr << "SQL error: " << e.what() << std::endl;
        return false;
    }
    catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
}

static uint32_t random_number() {
    std::random_device dev;

    std::mt19937 rng(dev());
    std::uniform_int_distribution<uint32_t> dist(1, INT32_MAX);
    return dist(rng);
}

msg_gen h_get_index_html(http::request<http::string_body>&& request_) {
    return prepare_response_with_file(std::move(request_), index_html, "text/html");
}

msg_gen h_get_auth_html(http::request<http::string_body>&& request_) {
    return prepare_response_with_file(std::move(request_), auth_html, "text/html");
}

msg_gen h_get_auth_js(http::request<http::string_body>&& request_) {
    return prepare_response_with_file(std::move(request_), auth_js, "text/javascript");
}

msg_gen h_post_auth_js(http::request<http::string_body>&& request_) {

    namespace pt = boost::property_tree;

    beast::error_code ec;
    boost::json::value req_json = boost::json::parse(request_.body(), ec);
    std::string resp_body{};

    try {
        std::string login{ req_json.at("username").as_string() };
        std::string password{ req_json.at("password").as_string() };

        // async login data validation
        decltype(auto) fut = std::async([&login, &password]() {
            return validateCreadentials(std::move(login), std::move(password));
        });

#if 0
        Net::Grpc::Connection::instance()->exchange("hello through gRPC");
#endif

        if (!fut.get()) {
            return wrong_request(http::status::unauthorized, errorOccured("Invalid login or password"), std::move(request_));
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

msg_gen h_get_register_html(http::request<http::string_body>&& request_) {
    return prepare_response_with_file(std::move(request_), register_html, "text/html");
}

msg_gen h_get_register_js(http::request<http::string_body>&& request_) {
    return prepare_response_with_file(std::move(request_), register_js, "text/javascript");
}

msg_gen h_post_register_js(http::request<http::string_body>&& request_) {

    namespace pt = boost::property_tree;

    beast::error_code ec;
    boost::json::value req_json = boost::json::parse(request_.body(), ec);
    std::string resp_body{};

    try {
        std::string login{ req_json.at("username").as_string() };
        std::string password{ req_json.at("password").as_string() };

        // async login data validation
        decltype(auto) fut = std::async([&login, &password]() {
            return validateCreadentials(std::move(login), std::move(password));
        });

        if (!fut.get()) {
            return wrong_request(http::status::unauthorized, errorOccured("Invalid login or password"), std::move(request_));
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

msg_gen h_get_16x16_icon(http::request<http::string_body>&& request_) {
    return prepare_response_with_file(std::move(request_), favicon_16x16, "image/png");
}

msg_gen h_get_32x32_icon(http::request<http::string_body>&& request_) {
    return prepare_response_with_file(std::move(request_), favicon_32x32, "image/png");
}
} // Net::Api
