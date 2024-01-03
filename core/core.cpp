
#include <spdlog/spdlog.h>

#include <boost/lexical_cast.hpp>

#include <atomic>
#include <concepts>
#include <vector>
#include <thread>

#include "core.h"
#include "config.h"
#include "http_conn.h"
#include "grpc_conn.h"
#include "types.h"
#include "thread_pool.h"

namespace App {

Core::Core()
    : threads_num(boost::thread::hardware_concurrency()),
    ios(threads_num),
    work(ios),
    pool(threads_num, work),
    signals(work.get_io_context(), SIGINT, SIGTERM) {

    // signals handler
    signals.async_wait([&work = work](const boost::system::error_code& error, int signal_number) {
        std::ignore = error;
        std::ignore = signal_number;
        work.get_io_context().stop();
    });

    spdlog::info("Core class constructor");
}

Core::~Core() {
    spdlog::info("Core class destructor");
}

const Core& Core::create() {
    static Core core;
    return core;
}

const Core& Core::config() const {

    using Config = App::Config;
    using namespace boost::asio;

    decltype(auto) config(Config::instance());

    spdlog::info("Start server listening {}:{}", config["HTTP_HOST"], config["HTTP_PORT"]);

    // create thread pool
    pool.config();

    // setup callback init http connection
    pool.callback([&work = work, host = ip::make_address(config["HTTP_HOST"]), port = boost::lexical_cast<u16>(config["HTTP_PORT"])]() {
        Net::Http::Connection::instance(work.get_io_context(), host, port)->setup_routing()->run();
    });

    // setup callback init grpc connection
    pool.callback([host = config["GRPC_HOST"], port = config["GRPC_PORT"]]() {
        decltype(auto) inst(Net::Grpc::Connection::instance());
        inst->config(host, port);
        inst->run();
    });

    return *this;
}

void Core::run() const noexcept {
    pool.run();
}
}