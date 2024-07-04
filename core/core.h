
#pragma once

#include <boost/asio/thread_pool.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/asio/post.hpp>

#include <memory>

#include "thread_pool.h"

namespace App {
class Core {
public:

    static const Core& create();

    Core(const Core&) = delete;
    Core(Core&&) = delete;
    const Core& operator=(const Core&) = delete;
    Core&& operator=(Core&&) = delete;

    ~Core();

    const Core& config() const;
    void run() const noexcept;

private:
    Core();

private:
    int _threads_num = 1;

    boost::asio::io_context _ios;
    mutable boost::asio::io_context::work _work;
    boost::asio::signal_set _signals;

    mutable ThreadPool _pool;
};
}