

#include "server.h"

namespace Net {

void Server::init(service& ios, const endpoint& endp)
{
    static std::unique_ptr<Server> server(new Server(ios, endp));
}

Server::Server(service& ios, const endpoint& endp)
    : ios(ios) 
    , acceptor(ios, endp)
    , socket(ios)
{
    spdlog::debug("Server class contructor");

    listen();
    ios.run();
    
}

Server::~Server() 
{
    spdlog::debug("Server class destructor");
}

void Server::listen() 
{
    acceptor.async_accept(
        socket, [&](boost::beast::error_code ec) 
        {
            if (!ec) 
            {
                spdlog::info("Connection accepted");
            }
            listen();
        }
    );
}

}
