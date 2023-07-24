
#include <spdlog/spdlog.h>

#include "config.h"
#include "core.h"

int main()
{
    spdlog::info("Welcome media server");

    try {


        auto& conf = App::Config::instance();

        auto major = conf.getValue("MAJOR");
        auto minor = conf.getValue("MINOR");
        auto patch = conf.getValue("PATCH");
        auto build = conf.getValue("BUILD");

        spdlog::info("Application version v.{}.{}.{}.{}", major, minor, patch, build);

        auto& app = App::Core::instance();
        app.run();
    }
    catch (const std::exception& ex) {
        spdlog::error("Somethig went wrong: {}", ex.what());
    }
    return 0;
}   