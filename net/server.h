
#ifndef NET_SERVER_H
#define NET_SERVER_H

#include "session.h"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

namespace Net {

class Server : public std::enable_shared_from_this<Server> {

    using endpoint = boost::asio::ip::tcp::endpoint;
    using service = boost::asio::io_service;

public:

    static std::shared_ptr<Server> init(service& ios, const endpoint& endp);

    Server() = delete;
    Server(const Server&) = delete;
    Server(Server&&) = delete;
    const Server& operator=(const Server&) = delete;
    Server&& operator=(Server&&) = delete;

    ~Server();

    void run() noexcept;

private:

    Server(service& ios, const endpoint& endp);

    void start_accept();
    void handle_accept(beast::error_code ec, tcp::socket socket);

private:

    boost::asio::io_service& ios;
    boost::asio::ip::tcp::acceptor acceptor_;

    std::shared_ptr<std::string const> doc_root_;
};
}

#endif // NET_SERVER_H
