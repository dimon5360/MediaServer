
#pragma once

#include "handlers/handlers.h"

namespace Net {
class Api {

public:

    static Handler::IRequest::msg_gen h_get_16x16_icon(Handler::http::request<Handler::http::string_body>&& request_);
    static Handler::IRequest::msg_gen h_get_32x32_icon(Handler::http::request<Handler::http::string_body>&& request_);

    static Handler::IRequest::msg_gen h_get_index(Handler::http::request<Handler::http::string_body>&& request_);
    static Handler::IRequest::msg_gen h_get_main(Handler::http::request<Handler::http::string_body>&& request_);
    static Handler::IRequest::msg_gen h_get_login(Handler::http::request<Handler::http::string_body>&& request_);

    static Handler::IRequest::msg_gen h_post_login(Handler::http::request<Handler::http::string_body>&& request_);
};

}