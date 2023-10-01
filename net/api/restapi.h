
#pragma once

#include "handlers/handlers.h"

namespace Net {
class Api {

public:

    static Handler::IRequest::msg_gen h_get_16x16_icon(Handler::http::request<Handler::http::string_body>&& request_);
    static Handler::IRequest::msg_gen h_get_32x32_icon(Handler::http::request<Handler::http::string_body>&& request_);

    static Handler::IRequest::msg_gen h_get_index_html(Handler::http::request<Handler::http::string_body>&& request_);

    static Handler::IRequest::msg_gen h_get_auth_html(Handler::http::request<Handler::http::string_body>&& request_);
    static Handler::IRequest::msg_gen h_get_auth_js(Handler::http::request<Handler::http::string_body>&& request_);
    static Handler::IRequest::msg_gen h_post_auth_js(Handler::http::request<Handler::http::string_body>&& request_);

    static Handler::IRequest::msg_gen h_get_register_html(Handler::http::request<Handler::http::string_body>&& request_);
    static Handler::IRequest::msg_gen h_get_register_js(Handler::http::request<Handler::http::string_body>&& request_);
    static Handler::IRequest::msg_gen h_post_register_js(Handler::http::request<Handler::http::string_body>&& request_);
};

}