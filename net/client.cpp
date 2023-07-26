
#include "client.h"

#include <spdlog/spdlog.h>

namespace Net {

Client::Client(boost::asio::io_service& ios) 
    : socket_(ios)
{
    spdlog::info("Client class constructor");
}

Client::~Client() 
{
    spdlog::info("Client class destructor");
}

void Client::run()
{

}

Client::cli_ptr Client::create(boost::asio::io_service& ios) 
{
    cli_ptr cli(new Client(ios));
    return cli;     
}

boost::asio::ip::tcp::socket& Client::socket() 
{
    return socket_;
}

}