
#include "irequest_handler.h"

#include <spdlog/spdlog.h>

namespace Net::Handler {

IRequest::IRequest() noexcept {
    spdlog::info("IRequest class constructor");
}

IRequest::~IRequest() {
    spdlog::info("IRequest class destructor");
}

http::message_generator IRequest::handle(http::request<http::string_body>&& req, const std::string& doc_root) {
    http::response<http::file_body> res;
    return res;
}

std::string IRequest::path_cat(beast::string_view base, beast::string_view path) {
    if (base.empty())
        return std::string(path);
    std::string result(base);
    char constexpr path_separator = '/';
    if (result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
    return result;
}
}