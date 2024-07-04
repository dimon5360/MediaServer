
#include <spdlog/spdlog.h>

#include "config.h"
#include "core.h"

#include <boost/process.hpp>

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

        App::Core::create().config().run();
    }
    catch (const std::exception& ex) {
        spdlog::error("The exception caught: {}", ex.what());
    }

    spdlog::info("Application terminated correctly");
    return 0;
}