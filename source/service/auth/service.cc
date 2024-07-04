
#include <exception>
#include <source_location>

#include "auth/service.h"

namespace Service::Auth {

constexpr bool verbose = false;

Instance::Instance() {
    if (verbose) spdlog::debug("auth service class constructor");
}

Instance::~Instance() {
    if (verbose) spdlog::debug("auth service class destructor");
}

void Instance::Init() const {
    if (verbose) {
        spdlog::info("called {} not implemented", std::source_location::current().function_name());
    }
}

void Instance::Run() const {
    if (verbose) {
        spdlog::info("called {} not implemented", std::source_location::current().function_name());
    }
}

} // namespace Service::Auth