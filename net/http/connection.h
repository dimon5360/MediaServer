
#ifndef NET_SERVER_H
#define NET_SERVER_H

#include "iconnection.h"
#include "session/session.h"

namespace Net::Http {

class Connection : public IConnection, public std::enable_shared_from_this<Connection> {

    using endpoint = boost::asio::ip::tcp::endpoint;
    using service = boost::asio::io_service;

public:

    static std::shared_ptr<Connection> init(service& ios, const endpoint& endp);

    Connection() = delete;
    Connection(const Connection&) = delete;
    Connection(Connection&&) = delete;
    const Connection& operator=(const Connection&) = delete;
    Connection&& operator=(Connection&&) = delete;

    ~Connection();

    void run() noexcept;
    std::shared_ptr<Connection> setup_routing();

private:

    Connection(service& ios, const endpoint& endp);

    void start_accept();
    void handle_accept(beast::error_code ec, tcp::socket socket);


private:

    boost::asio::io_service& ios;
    boost::asio::ip::tcp::acceptor acceptor_;

    std::shared_ptr<std::string const> doc_root_;
};
}

#endif // NET_SERVER_H
