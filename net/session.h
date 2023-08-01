
#ifndef NET_SESSION_H
#define NET_SESSION_H

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

namespace Net {

namespace beast = boost::beast;
namespace http = beast::http;
using tcp = boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {

public:

    explicit Session(tcp::socket&& socket,
        std::shared_ptr<std::string const> const& doc_root);

    Session() = delete;
    Session(const Session&) = delete;
    Session(Session&&) = delete;
    const Session& operator=(const Session&) = delete;
    Session&& operator=(Session&&) = delete;

    ~Session();

    void run();

private:

    void start_read();
    void handle_read(beast::error_code ec, std::size_t bytes_transferred);

    void start_write(http::message_generator&& msg);
    void handle_write(
        bool keep_alive,
        beast::error_code ec,
        std::size_t bytes_transferred);

    void fail(beast::error_code ec, std::string&& info);
    void close_session();

private:

    std::shared_ptr<std::string const> doc_root_;

    beast::tcp_stream stream_;
    beast::flat_buffer buffer_{ 8192 };

    http::request<http::string_body> request_;
    http::response<http::string_body> response_;
};

}

#endif // NET_SESSION_H