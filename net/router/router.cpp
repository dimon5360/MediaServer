
#include "router.h"
#include "session/session.h"
#include "api/rest/handler.h"

#include <spdlog/spdlog.h>
#include <boost/beast.hpp>

namespace Net {

void Router::init_routing() {

    using enum boost::beast::http::verb;

    set_handler<get>("/favicon-16x16.png", Api::Rest::h_get_16x16_icon);
    set_handler<get>("/favicon-32x32.png", Api::Rest::h_get_32x32_icon);

    set_handler<get>("", Api::Rest::h_get_index_html);
    set_handler<get>("/api/v1/index.html", Api::Rest::h_get_index_html);

    set_handler<get>("/api/v1/auth.html", Api::Rest::h_get_auth_html);
    set_handler<get>("/api/v1/auth.js", Api::Rest::h_get_auth_js);
    set_handler<post>("/api/v1/auth.js", Api::Rest::h_post_auth_js);

    set_handler<get>("/api/v1/register.html", Api::Rest::h_get_register_html);
    set_handler<get>("/api/v1/register.js", Api::Rest::h_get_register_js);
    set_handler<post>("/api/v1/register.js", Api::Rest::h_post_register_js);
}
}