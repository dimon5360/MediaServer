
#if 0
#ifndef NET_GRPC_CONNECTION_H
#define NET_GRPC_CONNECTION_H

#include "iconn.h"

#include <iostream>
#include <memory>
#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"

#include <grpcpp/grpcpp.h>

#include <storage.grpc.pb.h>

namespace Net::Grpc {

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

class GreeterClient {

    friend class Connection;

protected:
    GreeterClient() : stub_(nullptr) {

    }

    GreeterClient(std::shared_ptr<Channel> channel)
        : stub_(Greeter::NewStub(channel)) { }

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    std::string SayHello(const std::string& user) {
        // Data we are sending to the server.
        HelloRequest request;
        request.set_name(user);

        // Container for the data we expect from the server.
        HelloReply reply;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        Status status = stub_->SayHello(&context, request, &reply);

        // Act upon its status.
        if (status.ok()) {
            return reply.message();
        }
        else {
            std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
            return "RPC failed";
        }
    }

    bool IsInitied() {
        return isInited;
    }

private:
    std::unique_ptr<Greeter::Stub> stub_;
    bool isInited = false;
};

class Connection : public IConnection, public std::enable_shared_from_this<Connection> {

public:

    static std::shared_ptr<Connection> instance();

    Connection(const Connection&) = delete;
    Connection(Connection&&) = delete;
    const Connection& operator=(const Connection&) = delete;
    Connection&& operator=(Connection&&) = delete;

    ~Connection();

    void config(const std::string& host, const std::string& port);
    void exchange(const std::string& msg);
    void run();

private:

    Connection();

private:

    GreeterClient cli;
};
}

#endif // NET_GRPC_CONNECTION_H

#endif