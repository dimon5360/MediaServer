
#include <spdlog/spdlog.h>

#include "config.h"
#include "core.h"

namespace {
const std::string appEnv{ "../env/app.env" };
const std::string apiEnv{ "../env/api.env" };
const std::string dbEnv{ "../env/db.env" };
}

extern int server_main(int, char**);
extern int client_main();

int main() {
    spdlog::info("Welcome media server");

    try {

        decltype(auto) config = App::Config::instance().parseEnv(appEnv, apiEnv, dbEnv);

        auto major = config["MAJOR"];
        auto minor = config["MINOR"];
        auto patch = config["PATCH"];
        auto build = config["BUILD"];

        spdlog::info("Application version v.{}.{}.{}.{}", major, minor, patch, build);

        auto grpc_cli = std::thread([&]() {
            client_main();
        });

        auto grpc_srv = std::thread([&]() {
            server_main(1, NULL);
        });

        grpc_cli.join();
        grpc_srv.join();

        App::Core::create().run();
    }
    catch (const std::exception& ex) {
        spdlog::error("The exception caught: {}", ex.what());
    }
    return 0;
}