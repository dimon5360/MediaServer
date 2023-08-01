
#pragma once

#include <string>
#include <memory>
#include <fstream>
#include <vector>
#include <map>

#include <spdlog/spdlog.h>
#include <boost/algorithm/string.hpp>

#include "types.h"

namespace App {

class Config {

public:

    static std::shared_ptr<Config> instance();

    Config(const Config&) = delete;
    Config(Config&&) = delete;
    const Config& operator=(const Config&) = delete;
    Config&& operator=(Config&&) = delete;

    ~Config();

    template<typename ... Ts>
    void parseEnv(Ts ... args) const {

        std::vector<std::common_type_t<Ts...>> vec_{ args... };
        for (const auto& path : vec_) {

            std::fstream f(path, std::ios_base::in);
            if (!f.is_open()) {
                spdlog::error("Faild open config file");
                return;
            }

            typedef std::vector<std::string> Tokens;
            Tokens tokens;
            for (std::string line; getline(f, line); ) {
                if (line.empty() || line.starts_with("#"))
                    continue; // with '#' start comments

                boost::split(tokens, line, boost::is_any_of("="));
                m_.insert({ tokens[0], tokens[1] });
            }

            f.close();
        }
    }
    const std::string& getValue(std::string&& key) const;

private:

    Config();


public:

private:

    mutable std::map<std::string, std::string> m_;
};
}