
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

class Config : public std::enable_shared_from_this<Config> {

public:

    static std::shared_ptr<const Config> instance();

    Config(const Config&) = delete;
    Config(Config&&) = delete;
    const Config& operator=(const Config&) = delete;
    Config&& operator=(Config&&) = delete;

    ~Config();

    template<typename ... Ts>
    std::shared_ptr<const Config> parseEnv(Ts ... args) const {

        std::vector<std::common_type_t<Ts...>> vec_{ args... };
        for (const auto& file : vec_) {

            std::fstream f(file, std::ios_base::in);
            if (!f.is_open()) {
                spdlog::error("Faild open config file '{}", file);
                return shared_from_this();
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
        return shared_from_this();
    }

    template<typename T = std::string>
    const std::string& operator[](T&& key) const {
        return m_.at(key);
    }
private:

    Config();

private:

    mutable std::map<std::string, std::string> m_;
};
}