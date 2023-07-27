
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

    const int N = boost::thread::hardware_concurrency();

    boost::asio::io_service ios{ N };

    ip::tcp::endpoint endp{ipaddr, ipport};
    Server::init(ios, endp)->run();
    
    std::vector<std::thread> v;
    v.reserve(N - 1);
    for(auto i = N - 1; i > 0; --i)
        v.emplace_back(
        [&ios]
        {
            ios.run();
        });
    ios.run();
}
}