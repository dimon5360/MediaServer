
#include <spdlog/spdlog.h>
#include <boost/process.hpp>

#include "config.h"
#include "provider.h"

int main() {
    spdlog::info("Welcome media server");

    try {
        const char* pwd = std::getenv("PWD");
        assert(pwd != nullptr);

        decltype(auto) config = App::Config::instance().parseEnv(std::string(pwd) + "/.env");

        auto major = config["MAJOR"];
        auto minor = config["MINOR"];
        auto patch = config["PATCH"];
        auto build = config["BUILD"];

        spdlog::info("Application version v.{}.{}.{}.{}", major, minor, patch, build);

        Service::Provider provider;

        provider.AuthService();
    }
    catch (const std::exception& ex) {
        spdlog::error("The exception caught: {}", ex.what());
    }

    spdlog::info("Application terminated correctly");
    return 0;
}