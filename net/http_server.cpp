
#include "http_server.h"
#include "router.h"
#include "config.h"

#include <spdlog/spdlog.h>
#include <boost/bind.hpp>
#include <future>
#include <string>

namespace Net {

using namespace boost::asio;

namespace {
using method = boost::beast::http::verb;

const std::string index_html{ "../static/html/index.html" };

const auto wrongFile = [](std::string_view resource) -> decltype(auto) {
    return boost::str(boost::format("The resourse '%1%' was not found.") % resource);
};

const auto errorOccured = [](std::string_view err_msg) -> decltype(auto) {
    return boost::str(boost::format("An error occurred: '%1%'") % err_msg);
};
}

std::shared_ptr<HttpServer> HttpServer::init(service& ios, const endpoint& endp) {
    static std::shared_ptr<HttpServer> server(new HttpServer(ios, endp));
    return server;
}

HttpServer::HttpServer(service& ios, const endpoint& endp)
    : IServer()
    , ios(ios)
    , acceptor_(boost::asio::make_strand(ios)) {
    doc_root_ = std::make_shared<std::string const>("/");

    spdlog::info("HttpServer class contructor");
    beast::error_code ec;

    acceptor_.open(endp.protocol(), ec);
    if (ec) {
        spdlog::error("HttpServer error: {}", ec.message());
        return;
    }

    acceptor_.set_option(boost::asio::socket_base::reuse_address(true), ec);
    if (ec) {
        spdlog::error("HttpServer error: {}", ec.message());
        return;
    }

    acceptor_.bind(endp, ec);
    if (ec) {
        spdlog::error("HttpServer error: {}", ec.message());
        return;
    }

    acceptor_.listen(
        boost::asio::socket_base::max_listen_connections, ec);
    if (ec) {
        spdlog::error("HttpServer error: {}", ec.message());
        return;
    }
}

HttpServer::~HttpServer() {
    spdlog::info("HttpServer class destructor");
}

void HttpServer::run() noexcept {
    try {
        start_accept();
    }
    catch (const std::exception& ex) {
        spdlog::error("catch exception: {}", ex.what());
    }
}

std::string path_cat(beast::string_view base, beast::string_view path) {
    if (base.empty())
        return std::string(path);
    std::string result(base);

    if (result.back() == '/')
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
    return result;
}

std::shared_ptr<HttpServer> HttpServer::setup_routing() {
    decltype(auto) config = App::Config::instance();
    decltype(auto) router = Router::instance();

    router.setup_route<boost::beast::http::verb::get>((*config)["API_V1_INDEX"],
        [&](http::request<http::string_body>&& request_) -> Handler::IRequest::msg_gen {

        beast::error_code ec;
        http::file_body::value_type body;
        body.open(index_html.c_str(), beast::file_mode::scan, ec);

        if (beast::errc::no_such_file_or_directory == ec)
            return Handler::IRequest::wrong_request(wrongFile(request_.target()), std::move(request_));

        if (beast::errc::success != ec)
            return Handler::IRequest::wrong_request(errorOccured(ec.message()), std::move(request_));

        auto const size = body.size();

        if (request_.method() == http::verb::head) {
            http::response<http::empty_body> res{ http::status::ok, request_.version() };
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, "text/html");
            res.content_length(size);
            res.keep_alive(request_.keep_alive());
            return res;
        }

        auto header{ std::make_tuple(http::status::ok, request_.version()) };
        http::response<http::file_body> res{ std::piecewise_construct, std::make_tuple(std::move(body)), header };
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.content_length(size);
        res.keep_alive(request_.keep_alive());
        return res;
    }
    );

    return shared_from_this();
}

void HttpServer::start_accept() {
    spdlog::info("wait new client ...");

    acceptor_.async_accept(boost::asio::make_strand(ios),
        beast::bind_front_handler(
            &HttpServer::handle_accept,
            shared_from_this()));

}

void HttpServer::handle_accept(beast::error_code ec, tcp::socket socket) {
    if (ec) {
        spdlog::error("HttpServer error: {}", ec.message());
        return;
    }

    spdlog::info("Connection accepted");
    std::make_shared<Session>(std::move(socket))->run();
    start_accept();
}
}
