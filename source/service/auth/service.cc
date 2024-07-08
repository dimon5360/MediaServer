
#include <memory>
#include <source_location>

#include "service.h"
#include "core.h"

namespace Service {

constexpr bool verbose = false;

Authentication::Authentication() {
    spdlog::debug("authentication service class constructor");
}

Authentication::~Authentication() {
    spdlog::debug("authentication service class destructor");
}

interface_ptr Authentication::Instance() {
    return interface_ptr(new Authentication()); // TODO: fix calling new()
}

void Authentication::Init() const {
    if (verbose) {
        spdlog::info("called {} not implemented", std::source_location::current().function_name());
    }
}

void Authentication::Run() const {
    if (verbose) {
        spdlog::info("called {} not implemented", std::source_location::current().function_name());
    }

    App::Core::create().config().run();
}

} // namespace Service::Auth