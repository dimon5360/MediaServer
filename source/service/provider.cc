
#include "provider.h"
#include "auth/service.h"

#include <list>

namespace Service {

namespace {
constexpr bool verbose = true;
}

Provider::Provider() {
    if (verbose) spdlog::debug("provider class constructor");
}

Provider::~Provider() {
    if (verbose) spdlog::debug("provider class destructor");
}

const Provider& Provider::AuthService() const {
    // Authentication::Instance();
    // service.get()->Init();
    // service.get()->Run();

    _services.emplace_back(Authentication::Instance());
    return *this;
}

} // namespace Service