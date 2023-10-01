
#include "router.h"
#include "session/session.h"
#include "api/restapi.h"

#include <spdlog/spdlog.h>
#include <boost/beast.hpp>

namespace Net {

void Router::init_routing() {

    using enum boost::beast::http::verb;

    add_routing<get>("/favicon-16x16.png", Api::h_get_16x16_icon);
    add_routing<get>("/favicon-32x32.png", Api::h_get_32x32_icon);

    add_routing<get>("", Api::h_get_index);
    add_routing<get>("/api/v1/main.html", Api::h_get_main);
    add_routing<get>("/api/v1/login.js", Api::h_get_login);

    add_routing<post>("/api/v1/login.js", Api::h_post_login);
}
}