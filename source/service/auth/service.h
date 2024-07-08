
#pragma once

#include "interface.h"
#include "provider.h"

#include <utility>

namespace Service {

class Authentication : public Service::Interface {

private:

    Authentication();

public:

    static interface_ptr Instance();

    Authentication(const Authentication&) = delete;
    Authentication& operator=(const Authentication&) = delete;

    Authentication(Authentication&&) = delete;
    Authentication& operator=(Authentication&&) = delete;

    ~Authentication() override;

    void Init() const override;
    void Run() const override;
};

} // namespace Service::Auth