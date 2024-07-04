
#include "config.h"

namespace App {

const Config& Config::instance() {
    static Config config;
    return config;
}

Config::Config() {
    spdlog::debug("Config class constructor");
}

Config::~Config() {
    spdlog::debug("Config class destructor");
}

}