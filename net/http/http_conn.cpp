
#include "types.h"
#include "http_conn.h"
#include "router/router.h"

#include <spdlog/spdlog.h>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

namespace Net::Http {

namespace ip = boost::asio::ip;

std::shared_ptr<Connection> Connection::instance(service& ios, const ip::address& host, const u16 port) {
    static endpoint endp{ host, port };
    static std::shared_ptr<Connection> connection(new Connection(ios, endp));
    return connection;
}

Connection::Connection(service& ios, const endpoint& endp)
    : IConnection()
    , ios(ios)
    , acceptor_(boost::asio::make_strand(ios)) {
    doc_root_ = std::make_shared<std::string const>("/");

    spdlog::info("HTTP connection class contructor");
    beast::error_code ec;

    acceptor_.open(endp.protocol(), ec);
    if (ec) {
        spdlog::error("HTTP connection error: {}", ec.message());
        return;
    }

    acceptor_.set_option(boost::asio::socket_base::reuse_address(true), ec);
    if (ec) {
        spdlog::error("HTTP connection error: {}", ec.message());
        return;
    }

    acceptor_.bind(endp, ec);
    if (ec) {
        spdlog::error("HTTP connection error: {}", ec.message());
        return;
    }

    acceptor_.listen(
        boost::asio::socket_base::max_listen_connections, ec);
    if (ec) {
        spdlog::error("HTTP connection error: {}", ec.message());
        return;
    }
}

Connection::~Connection() {
    spdlog::info("HTTP connection class destructor");
}

void Connection::run() noexcept {
    try {
        start_accept();
    }
    catch (const std::exception& ex) {
        spdlog::error("catch exception: {}", ex.what());
    }
}

std::shared_ptr<Connection> Connection::setup_routing() {
    Router::instance().init_routing();
    return shared_from_this();
}

void Connection::start_accept() {
    spdlog::info("wait new client ...");

    acceptor_.async_accept(boost::asio::make_strand(ios),
        beast::bind_front_handler(
            &Connection::handle_accept,
            shared_from_this()));
}

void Connection::handle_accept(beast::error_code ec, tcp::socket socket) {
    if (ec) {
        spdlog::error("Connection error: {}", ec.message());
        return;
    }

    spdlog::info("Connection accepted");
    std::make_shared<Session>(std::move(socket))->run();
    start_accept();
}
}
