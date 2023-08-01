
#include "router.h"

#include <spdlog/spdlog.h>

namespace Net::Router {

Router::Router() {
    spdlog::info("Router class constructor");
}

Router::~Router() {
    spdlog::info("Router class destructor");
}

std::shared_ptr<Router> Router::create() {
    static std::shared_ptr<Router> router(new Router());
    return router;
}

} // namespace Net::Router
