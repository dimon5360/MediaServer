
#pragma once

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

namespace Handler {

namespace beast = boost::beast;
namespace http = beast::http;

class Request {

public:
    Request(http::request<http::string_body> && req_, const std::string& doc_root);
    ~Request();

    void handle();

private:
    
    http::request<http::string_body> request_;
    std::string doc_root_;
};

}