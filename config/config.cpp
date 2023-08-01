
#include "config.h"

namespace App {

std::shared_ptr<const Config> Config::instance() {
    static std::shared_ptr<const Config> config(new Config());
    return config;
}

Config::Config() {
    spdlog::debug("Config class constructor");
}

Config::~Config() {
    spdlog::debug("Config class destructor");
}

}