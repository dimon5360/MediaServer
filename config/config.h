
#pragma once

#include <string>
#include <memory>
#include <map>

#include <spdlog/spdlog.h>

#include "types.h"

namespace App {

class Config {

public:

    static const Config& instance();

    Config(const Config&) = delete;
    Config(Config&&) = delete;
    const Config& operator=(const Config&) = delete;
    Config&& operator=(Config&&) = delete;

    ~Config();

    const std::string& getValue(std::string && key) const;

private:

    Config();

    void initEnv();

public:

private:

    std::map<std::string, std::string> m_;
};
}