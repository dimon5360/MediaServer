
#pragma once

#include <spdlog/spdlog.h>

namespace Service {

class Interface {

public:

    Interface() {
        spdlog::debug("base service class constructor");
    }

    Interface(const Interface&) = delete;
    Interface(Interface&&) = delete;
    Interface& operator= (const Interface&) = delete;
    Interface& operator= (Interface&&) = delete;

    virtual ~Interface() {
        spdlog::debug("base service class destructor");
    }

    virtual void Init() const = 0;
    virtual void Run() const = 0;
};

using interface_ptr = std::shared_ptr<Interface>;

} // namespace Service