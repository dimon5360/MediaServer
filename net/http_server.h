
#ifndef NET_SERVER_H
#define NET_SERVER_H

#include "iserver.h"
#include "session/session.h"

namespace Net {

class HttpServer : public IServer, public std::enable_shared_from_this<HttpServer> {

    using endpoint = boost::asio::ip::tcp::endpoint;
    using service = boost::asio::io_service;

public:

    static std::shared_ptr<HttpServer> init(service& ios, const endpoint& endp);

    HttpServer() = delete;
    HttpServer(const HttpServer&) = delete;
    HttpServer(HttpServer&&) = delete;
    const HttpServer& operator=(const HttpServer&) = delete;
    HttpServer&& operator=(HttpServer&&) = delete;

    ~HttpServer();

    void run() noexcept;
    std::shared_ptr<HttpServer> setup_routing();

private:

    HttpServer(service& ios, const endpoint& endp);

    void start_accept();
    void handle_accept(beast::error_code ec, tcp::socket socket);


private:

    boost::asio::io_service& ios;
    boost::asio::ip::tcp::acceptor acceptor_;

    std::shared_ptr<std::string const> doc_root_;
};
}

#endif // NET_SERVER_H
