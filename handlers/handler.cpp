
#include "handler.h"

#include <spdlog/spdlog.h>

namespace Handler {

Request::Request(http::request<http::string_body>&& req, const std::string& doc_root)
    : request_(std::move(req))
    , doc_root_(doc_root)
{
    spdlog::info("Request class constructor");
}

Request::~Request()
{
    spdlog::info("Request class destructor");
}

void Request::handle()
{

}

}
