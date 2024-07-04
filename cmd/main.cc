
#include <spdlog/spdlog.h>

#include "config.h"
#include "core.h"

#include <boost/process.hpp>

namespace 
{
    const std::string appEnv{ "../env/app.env" };
    const std::string apiEnv{ "../env/api.env" };
    const std::string dbEnv{ "../env/db.env" };
}

int main() 
{
    spdlog::info("Welcome media server");

    try {

        const auto env_p = std::string(std::getenv("PWD"));
        decltype(auto) config = App::Config::instance().parseEnv(env_p + "/.env");

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