
#include <fstream>
#include <vector>

#include <boost/algorithm/string.hpp>

#include "config.h"

namespace App {

namespace {
const std::string env{"../env/app.env"};
}

const Config& Config::instance() 
{
    static std::shared_ptr<Config> config(new Config());
    return *config;
}

Config::Config()
{
    spdlog::debug("Config class constructor");

    initEnv();
}

Config::~Config() 
{
    spdlog::debug("Config class destructor");
}

const std::string& Config::getValue(std::string && key) const 
{
    return m_.at(key);
}

void Config::initEnv() 
{
    std::fstream f(env, std::ios_base::in);
    if (!f.is_open()) 
    {
        spdlog::error("Faild open config file");
        return;
    }

    typedef std::vector<std::string> Tokens;
    Tokens tokens;
    for (std::string line; getline(f, line); ) 
    {
        if (line.empty() || line.starts_with("#"))
            continue; // with '#' start comments

        boost::split(tokens, line, boost::is_any_of("="));
        m_.insert({tokens[0], tokens[1]});
    }

    f.close();
}
}