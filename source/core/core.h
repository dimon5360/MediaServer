
#pragma once

#include <boost/asio/thread_pool.hpp>

#include <memory>

namespace App {
class Core : public std::enable_shared_from_this<Core> {
public:

    static std::shared_ptr<Core> create();

    Core(const Core&) = delete;
    Core(Core&&) = delete;
    const Core& operator=(const Core&) = delete;
    Core&& operator=(Core&&) = delete;

    ~Core();

    void run() const noexcept;

private:
    Core();

private:

};
}