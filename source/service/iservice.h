
#pragma once

#include <spdlog/spdlog.h>

namespace Service {

constexpr bool verbose = false;

class Interface {

public:

    Interface(const Interface&) = delete;
    Interface(Interface&&) = delete;
    Interface& operator= (const Interface&) = delete;
    Interface& operator= (Interface&&) = delete;

    Interface() {
        if (verbose) spdlog::debug("base service class constructor");
    }

    virtual ~Interface() {
        if (verbose) spdlog::debug("base service class destructor");
    }

    virtual void Init() const = 0;
    virtual void Run() const = 0;
};

} // Service