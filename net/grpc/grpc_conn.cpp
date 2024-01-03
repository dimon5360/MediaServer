
#include "grpc_conn.h"

namespace Net::Grpc {

std::shared_ptr<Connection> Connection::instance() {
    static std::shared_ptr<Connection> conn(new Connection());
    return conn;
}

Connection::Connection() {
    spdlog::info("gRPC connection class constructor");
}

Connection::~Connection() {
    spdlog::info("gRPC connection class destructor");
}

void Connection::config(const std::string& host, const std::string& port) {
    static std::string target_str = host + ":" + port;

    if (!cli.IsInitied()) {
        cli = GreeterClient(grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
    }
}

void Connection::run() {
    spdlog::info("Run gRPC client");
}

void Connection::exchange(const std::string& msg) {
    std::string reply = cli.SayHello(msg);
    std::cout << "Got reply: " << reply << std::endl;
}

}