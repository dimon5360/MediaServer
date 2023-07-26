
#ifndef NET_SERVER_H
#define NET_SERVER_H

#include "client.h"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

namespace Net {

class Server {

    using endpoint = boost::asio::ip::tcp::endpoint;
    using service = boost::asio::io_service;

public:

    static void init(service& ios, const endpoint& endp);

    Server() = delete;
    Server(const Server&) = delete;
    Server(Server&&) = delete;
    const Server& operator=(const Server&) = delete;
    Server&& operator=(Server&&) = delete;

    ~Server();

private:

    Server(service& ios, const endpoint& endp);

    void listen();

private:
    static std::shared_ptr<Server> server;

    boost::asio::io_service& ios;
    boost::asio::ip::tcp::acceptor acceptor;
};
}

#endif // NET_SERVER_H
