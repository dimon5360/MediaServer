
#include "thread_pool.h"

#include <spdlog/spdlog.h>
#include <boost/asio/post.hpp>

namespace App {

ThreadPool::ThreadPool(const int num, boost::asio::io_context::work& work) :
    num(num),
    work(work) {

}

void ThreadPool::run() {
    threads.join_all();
}

void ThreadPool::config() {

    for (int i = 0; i < num; ++i) { // threads number
        threads.create_thread([&work = work]() {
            work.get_io_context().run();
        });
    }
}

void ThreadPool::callback(std::function<void()> f) {
    boost::asio::post(work.get_io_context(), f);
}
}