
#include "router.h"
#include "config.h"

#include <spdlog/spdlog.h>

namespace Net {

Router::Router() {
    spdlog::info("Router class constructor");

    initRouting();
}

Router::~Router() {
    spdlog::info("Router class destructor");
}

std::shared_ptr<Router> Router::instance() {
    static std::shared_ptr<Router> router(new Router());
    return router;
}

void Router::initRouting() {

    try {
        decltype(auto) config = App::Config::instance();
        api_.insert({ (*config)["API_V1_INDEX"], std::make_shared<Handler::GetRequest>() });
        api_.insert({ (*config)["API_V1_MAIN"], std::make_shared<Handler::GetRequest>() });

    }
    catch (const std::exception& ex) {
        spdlog::error("The exception caught: {}", ex.what());
    }
}

} // namespace Net::Router
