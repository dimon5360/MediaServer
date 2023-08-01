
#include "config.h"

namespace App {

std::shared_ptr<Config> Config::instance() {
    static std::shared_ptr<Config> config(new Config());
    return config;
}

Config::Config() {
    spdlog::debug("Config class constructor");
}

Config::~Config() {
    spdlog::debug("Config class destructor");
}

const std::string& Config::getValue(std::string&& key) const {
    return m_.at(key);
}
}