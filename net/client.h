
#ifndef NET_CLIENT_H
#define NET_CLIENT_H

#include <memory>

#include <spdlog/spdlog.h>
#include <boost/asio.hpp>

namespace Net {

class Client {

private:

    Client(boost::asio::io_service& ios);

public:

    using cli_ptr = std::unique_ptr<Client>;

    Client() = delete;
    Client(const Client&) = delete;
    Client(Client&&) = delete;
    const Client& operator=(const Client&) = delete;
    Client&& operator=(Client&&) = delete;

    ~Client();

    boost::asio::ip::tcp::socket& socket();
    void run();

protected:

    static cli_ptr create(boost::asio::io_service& ios);

private:

    boost::asio::ip::tcp::socket socket_;

    size_t deviceSerialNumber = 0;

    friend class ClientFactory;
};

class ClientFactory {

    ClientFactory() 
    {
        spdlog::info("ClientFactory class constructor");
    }

public:

    static const ClientFactory& instance()
    {
        static ClientFactory factory;
        return factory;
    }

    ClientFactory(const ClientFactory&) = delete;
    ClientFactory(ClientFactory&&) = delete;
    const ClientFactory& operator=(const ClientFactory&) = delete;
    ClientFactory&& operator=(ClientFactory&&) = delete;

    ~ClientFactory() 
    {
        spdlog::info("ClientFactory class destructor");
    }
    
    Client::cli_ptr newClient(boost::asio::io_service& ios) const 
    {
        return Client::create(ios);
    }
};

}

#endif // NET_CLIENT_H