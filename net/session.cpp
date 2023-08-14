
#include "session.h"
#include "router.h"

#include <boost/asio/buffer.hpp>
#include <spdlog/spdlog.h>
#include <memory>
#include <iostream>

namespace Net {

using namespace boost::asio;

Session::Session(tcp::socket&& socket_,
    std::shared_ptr<std::string const> const& doc_root)
    : stream_(std::move(socket_))
    , doc_root_(doc_root) {

    spdlog::info("Session class constructor");
}

Session::~Session() {
    spdlog::info("Session class destructor");
}

void Session::run() {
    boost::asio::dispatch(stream_.get_executor(),
        beast::bind_front_handler(
            &Session::start_read,
            shared_from_this()
        ));
}

void Session::start_read() {

    request_ = {};
    stream_.expires_after(std::chrono::seconds(30));

    boost::beast::http::async_read(
        stream_,
        buffer_,
        request_,
        beast::bind_front_handler(
            &Session::handle_read,
            shared_from_this()));
}

void Session::handle_read(beast::error_code ec, std::size_t bytes_transferred) {
    std::ignore = bytes_transferred;

    if (http::error::end_of_stream == ec)
        return close_session();

    if (ec)
        return fail(ec, __func__);

    try 
    {
        start_write(Router::instance()->execute(std::move(request_), *doc_root_));
    } 
    catch (std::exception &ex) 
    {
        spdlog::info("Exteption caught: {}", ex.what());
    }

}

void Session::start_write(http::message_generator&& msg) {
    bool keep_alive = msg.keep_alive();

    beast::async_write(
        stream_,
        std::move(msg),
        beast::bind_front_handler(
            &Session::handle_write, shared_from_this(), keep_alive));
}

void Session::handle_write(
    bool keep_alive,
    beast::error_code ec,
    std::size_t bytes_transferred) 
{

    boost::ignore_unused(bytes_transferred);

    if (ec)
        return fail(ec, __func__);

    if (!keep_alive)
        return close_session();

    start_read();
}

void Session::fail(beast::error_code ec, std::string&& info) {
    spdlog::error("Got sesssion error {} in function {}", ec.message(), info);
}

void Session::close_session() {
    beast::error_code ec;
    stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
}

}