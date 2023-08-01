
#include <spdlog/spdlog.h>

#include "config.h"
#include "core.h"

namespace {
const std::string appEnv{ "../env/app.env" };
const std::string apiEnv{ "../env/api.env" };
}

int main() {
    spdlog::info("Welcome media server");

    try {

        auto config = App::Config::instance();

        config->parseEnv(appEnv, apiEnv);
        // config->parseEnv(apiEnv);

        auto major = config->getValue("MAJOR");
        auto minor = config->getValue("MINOR");
        auto patch = config->getValue("PATCH");
        auto build = config->getValue("BUILD");

        spdlog::info("Application version v.{}.{}.{}.{}", major, minor, patch, build);

        App::Core::create()->run();
    }
    catch (const std::exception& ex) {
        spdlog::error("Somethig went wrong: {}", ex.what());
    }
    return 0;
}