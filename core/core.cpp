
#include <spdlog/spdlog.h>

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/asio/post.hpp>
#include <boost/lexical_cast.hpp>

#include <atomic>
#include <concepts>

#include "core.h"
#include "config.h"
#include "server.h"
#include "types.h"

namespace App {

Core::Core() {
    spdlog::info("Config class constructor");
}

Core::~Core() {
    spdlog::info("Config class destructor");
}

const Core& Core::instance() {
    static Core core;
    return core;
}

void Core::run() const noexcept {

    using Config = App::Config;
    using Server = Net::Server;
    using namespace boost::asio;

    const std::string host{ Config::instance().getValue("HOST") };
    const std::string port{ Config::instance().getValue("PORT") };

    spdlog::info("Start server listening {}:{}", host, port);

    const ip::address ipaddr = ip::make_address(host);
    const u16 ipport = boost::lexical_cast<u16>(port);

    io_context ioc;
    ip::tcp::endpoint endp{ipaddr, ipport};

    boost::thread_group threads;
    io_context::work work(ioc);
    signal_set signals(work.get_io_context(), SIGINT, SIGTERM);

    for (unsigned int i = 0; i < boost::thread::hardware_concurrency(); ++i) {
        threads.create_thread([&]() {
            work.get_io_context().run();
        });
    }

    boost::asio::post(work.get_io_context(), [&]() {
        Net::Server::init(work.get_io_context(), endp);
    });

    signals.async_wait([&](const boost::system::error_code& error, int signal_number) {
        work.get_io_context().stop();
    });

    threads.join_all();
}
}