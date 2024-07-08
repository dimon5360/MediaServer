
#pragma once

#include "interface.h"
#include "auth/service.h"

#include <memory>
#include <list>

namespace Service {

class Provider {

public:

    Provider();
    ~Provider();

    const Provider& AuthService() const;

private:

    mutable std::list<interface_ptr> _services;
};

} // namespace Service