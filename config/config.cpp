
#include "config.h"

namespace App {

const Config& Config::instance() {
    static std::shared_ptr<Config> config(new Config());
    return *config.get();
}

Config::Config() {
    spdlog::debug("Config class constructor");
}

Config::~Config() {
    spdlog::debug("Config class destructor");
}

}