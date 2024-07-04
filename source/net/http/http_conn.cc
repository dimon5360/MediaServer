
#include "types.h"
#include "http_conn.h"
#include "router/router.h"

#include <boost/lexical_cast.hpp>

#include <spdlog/spdlog.h>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

namespace Net::Http {

namespace ip = boost::asio::ip;

std::shared_ptr<Connection> Connection::instance(service& ios) {
    static std::shared_ptr<Connection> connection(new Connection(ios));
    return connection;
}

Connection::Connection(service& ios)
    : IConnection()
    , ios(ios)
    , acceptor_(boost::asio::make_strand(ios)) {

    doc_root_ = std::make_shared<std::string const>("/");
    spdlog::info("HTTP connection class contructor");
}

Connection::~Connection() {
    spdlog::info("HTTP connection class destructor");
}


void Connection::config(const std::string& host, const std::string& port) {
    static endpoint endp{ ip::make_address(host), boost::lexical_cast<u16>(port) };

    beast::error_code ec;

    acceptor_.open(endp.protocol(), ec);
    if (ec) {
        spdlog::error("HTTP connection opening error: {}", ec.message());
        return;
    }

    acceptor_.set_option(boost::asio::socket_base::reuse_address(true), ec);
    if (ec) {
        spdlog::error("HTTP connection setting option error: {}", ec.message());
        return;
    }

    acceptor_.bind(endp, ec);
    if (ec) {
        spdlog::error("HTTP connection binding error: {}", ec.message());
        return;
    }

    acceptor_.listen(boost::asio::socket_base::max_listen_connections, ec);
    if (ec) {
        spdlog::error("HTTP connection listening error: {}", ec.message());
        return;
    }

    Router::instance().init_routing();
}

void Connection::run() noexcept {
    try {
        start_accept();
    }
    catch (const std::exception& ex) {
        spdlog::error("catch exception: {}", ex.what());
    }
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
