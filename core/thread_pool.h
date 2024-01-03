
#ifndef CORE_THREAD_POOL_H
#define CORE_THREAD_POOL_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/asio/thread_pool.hpp>

class ThreadPool {

public:

    explicit ThreadPool(const int num, boost::asio::io_context::work& work);

    void run();
    void config();

    void callback(std::function<void()> f);

private:

    int num;
    boost::asio::io_context::work work;
    boost::thread_group threads;
};

#endif // CORE_THREAD_POOL_H