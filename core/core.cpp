
#include <spdlog/spdlog.h>

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/asio/post.hpp>
#include <boost/lexical_cast.hpp>

#include <atomic>
#include <concepts>
#include <vector>
#include <thread>

#include "core.h"
#include "config.h"
#include "connection.h"
#include "types.h"

namespace App {

Core::Core() {
    spdlog::info("Config class constructor");
}

Core::~Core() {
    spdlog::info("Config class destructor");
}

const Core& Core::create() {
    static Core core;
    return core;
}

void Core::run() const noexcept {

    using Config = App::Config;
    using namespace boost::asio;

    decltype(auto) config(Config::instance());
    const std::string host{ config["HOST"] };
    const std::string port{ config["PORT"] };

    spdlog::info("Start server listening {}:{}", host, port);

    const ip::address ipaddr = ip::make_address(host);
    const u16 ipport = boost::lexical_cast<u16>(port);

    const int N = boost::thread::hardware_concurrency();

    boost::asio::io_service ios{ N };
    ip::tcp::endpoint endp{ ipaddr, ipport };

    boost::thread_group threads;
    io_context::work work(ios);
    signal_set signals(work.get_io_context(), SIGINT, SIGTERM);

    for (int i = 0; i < N; ++i) {
        threads.create_thread([&work]() {
            work.get_io_context().run();
        });
    }

    boost::asio::post(work.get_io_context(), [&work, &endp]() {
        Net::Http::Connection::init(work.get_io_context(), endp)->setup_routing()->run();
    });

    signals.async_wait([&work](const boost::system::error_code& error, int signal_number) {
        std::ignore = error;
        std::ignore = signal_number;
        work.get_io_context().stop();
    });

    threads.join_all();
}
}