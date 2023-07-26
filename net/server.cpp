
#include <spdlog/spdlog.h>
#include <boost/bind.hpp>

#include "server.h"

namespace Net {

using namespace boost::asio;

void Server::init(service& ios, const endpoint& endp)
{
    static std::unique_ptr<Server> server(new Server(ios, endp));
}

Server::Server(service& ios, const endpoint& endp)
    : ios(std::ref(ios)) 
    , acceptor(ios, endp)
{
    spdlog::info("Server class contructor");

    listen();
    ios.run();
    
}

Server::~Server() 
{
    spdlog::info("Server class destructor");
}

void Server::listen() 
{
    spdlog::info("wait new client ...");

    Client::cli_ptr client = ClientFactory::instance().newClient(ios);

    acceptor.async_accept(client->socket(), [&](boost::system::error_code ec) {
        if (!ec) 
        {
            client->run();
            spdlog::info("Connection accepted");
        }
        listen();
    });
}
}
