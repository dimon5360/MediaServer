
#pragma once

#include "iservice.h"

namespace Service::Auth {

class Instance final : public Service::Interface {

public:

    Instance();
    ~Instance() override;

    void Init() const override;
    void Run() const override;
};

} // namespace Service::Auth