
#include <spdlog/spdlog.h>
#include <boost/bind.hpp>

#include "server.h"
#include "router.h"
#include "config.h"

namespace Net {

using namespace boost::asio;

namespace {
    using method = boost::beast::http::verb;
}

const std::shared_ptr<Server> Server::init(service& ios, const endpoint& endp) {
    static std::shared_ptr<Server> server(new Server(ios, endp));
    return server;
}

Server::Server(service& ios, const endpoint& endp)
    : ios(ios)
    , acceptor_(boost::asio::make_strand(ios))
    , doc_root_(std::make_shared<std::string const>("/")) {

    spdlog::info("Server class contructor");
    beast::error_code ec;

    acceptor_.open(endp.protocol(), ec);
    if (ec) {
        spdlog::error("Server error: {}", ec.message());
        return;
    }

    acceptor_.set_option(boost::asio::socket_base::reuse_address(true), ec);
    if (ec) {
        spdlog::error("Server error: {}", ec.message());
        return;
    }

    acceptor_.bind(endp, ec);
    if (ec) {
        spdlog::error("Server error: {}", ec.message());
        return;
    }

    acceptor_.listen(
        boost::asio::socket_base::max_listen_connections, ec);
    if (ec) {
        spdlog::error("Server error: {}", ec.message());
        return;
    }
}

Server::~Server() {
    spdlog::info("Server class destructor");
}

void Server::run() noexcept {
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
    char constexpr path_separator = '/';
    if (result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
    return result;
}

const std::shared_ptr<Server> Server::setup_routing() 
{
    decltype(auto) config = App::Config::instance();
    decltype(auto) router = Router::instance();
    
    router->setup_route((*config)["API_V1_INDEX"], std::make_shared<Handler::GetRequest>(
        [&](http::request<http::string_body>&& request_, const std::string& doc_root_) -> Handler::http::message_generator
        {
            std::string path = path_cat(doc_root_, request_.target());
            if (request_.target().back() == '/')
                path.append("index.html");

            beast::error_code ec;
            http::file_body::value_type body;
            body.open(path.c_str(), beast::file_mode::scan, ec);

            if (beast::errc::no_such_file_or_directory == ec)
                return Handler::IRequest::wrong_request(boost::format("The resourse '%1%' was not found.") % request_.target(), request_);

            if (ec)
                return Handler::IRequest::wrong_request(boost::format("An error occurred: '%1%'") % ec.message(), request_);

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
    ));

    router->setup_route((*config)["API_V1_MAIN"], std::make_shared<Handler::GetRequest>(
        [&](http::request<http::string_body>&& request_, const std::string& doc_root_) -> Handler::http::message_generator
        {
            std::string path = path_cat(doc_root_, request_.target());
            if (request_.target().back() == '/')
                path.append("index.html");

            beast::error_code ec;
            http::file_body::value_type body;
            body.open(path.c_str(), beast::file_mode::scan, ec);

            if (beast::errc::no_such_file_or_directory == ec)
                return Handler::IRequest::wrong_request(boost::format("The resourse '%1%' was not found.") % request_.target(), request_);

            if (ec)
                return Handler::IRequest::wrong_request(boost::format("An error occurred: '%1%'") % ec.message(), request_);

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
    ));

    return shared_from_this();
}

void Server::start_accept() {
    spdlog::info("wait new client ...");

    acceptor_.async_accept(boost::asio::make_strand(ios),
        beast::bind_front_handler(
            &Server::handle_accept,
            shared_from_this()));

}

void Server::handle_accept(beast::error_code ec, tcp::socket socket) {
    if (ec) {
        spdlog::error("Server error: {}", ec.message());
        return;
    }

    spdlog::info("Connection accepted");
    std::make_shared<Session>(std::move(socket), doc_root_)->run();
    start_accept();
}
}
