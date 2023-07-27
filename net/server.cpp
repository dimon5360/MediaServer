
#include <spdlog/spdlog.h>
#include <boost/bind.hpp>

#include "server.h"

namespace Net {

using namespace boost::asio;

std::shared_ptr<Server> Server::init(service& ios, const endpoint& endp)
{
    static std::shared_ptr<Server> server(new Server(ios, endp));
    return server;
}

Server::Server(service& ios, const endpoint& endp)
    : ios(ios) 
    , acceptor_(boost::asio::make_strand(ios))
    , doc_root_(std::make_shared<std::string const>("/"))
{
    spdlog::info("Server class contructor");
    beast::error_code ec;

    acceptor_.open(endp.protocol(), ec);
    if(ec) {
        spdlog::error("Server error: {}", ec.message());
        return;
    }

    acceptor_.set_option(boost::asio::socket_base::reuse_address(true), ec);
    if(ec) {
        spdlog::error("Server error: {}", ec.message());
        return;
    }
    
    acceptor_.bind(endp, ec);
    if(ec) {
        spdlog::error("Server error: {}", ec.message());
        return;
    }

    acceptor_.listen(
        boost::asio::socket_base::max_listen_connections, ec);
    if(ec) {
        spdlog::error("Server error: {}", ec.message());
        return;
    }
}

Server::~Server() 
{
    spdlog::info("Server class destructor");
}

void Server::run() noexcept
{
    try {

        start_accept();
    }
    catch(const std::exception& ex) {

        spdlog::error("catch exception: {}", ex.what());
    }
}

void Server::start_accept() 
{
    spdlog::info("wait new client ...");

    acceptor_.async_accept(boost::asio::make_strand(ios), 
        beast::bind_front_handler(
            &Server::handle_accept, 
            shared_from_this()));

}

void Server::handle_accept(beast::error_code ec, tcp::socket socket) 
{        
    if (ec) {
        spdlog::error("Server error: {}", ec.message());
        return;
    }

    spdlog::info("Connection accepted");
    std::make_shared<Session>(std::move(socket), doc_root_)->run();
    start_accept();
}
}
