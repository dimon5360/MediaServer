
#include "router.h"
#include "session/session.h"
#include "http/api/handler.h"

#include <spdlog/spdlog.h>
#include <boost/beast.hpp>

namespace Net {

void Router::init_routing() {

    using enum boost::beast::http::verb;

    set_handler<get>("/favicon-16x16.png", Http::Api::h_get_16x16_icon);
    set_handler<get>("/favicon-32x32.png", Http::Api::h_get_32x32_icon);

    set_handler<get>("", Http::Api::h_get_index_html);
    set_handler<get>("/api/v1/index.html", Http::Api::h_get_index_html);

    set_handler<get>("/api/v1/auth.html", Http::Api::h_get_auth_html);
    set_handler<get>("/api/v1/auth.js", Http::Api::h_get_auth_js);
    set_handler<post>("/api/v1/auth.js", Http::Api::h_post_auth_js);

    set_handler<get>("/api/v1/register.html", Http::Api::h_get_register_html);
    set_handler<get>("/api/v1/register.js", Http::Api::h_get_register_js);
    set_handler<post>("/api/v1/register.js", Http::Api::h_post_register_js);
}
}