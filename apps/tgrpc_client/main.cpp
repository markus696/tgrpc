#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>

#include "rpc_model/echo_service.pb.h"
#include "rpc_model/echo_service.grpc.pb.h"

namespace rpc::echo {
    using rpc_model::echo::EchoRequest;
    using rpc_model::echo::EchoReply;
    using rpc_model::echo::Api;

    class Client {
    public:
        Client(std::shared_ptr<grpc::Channel> channel)
        : stub_(rpc_model::echo::Api::NewStub(channel)) {}

        std::string Echo(const std::string& msg) {
            EchoRequest request;
            request.set_message(msg);
            EchoReply reply;
            grpc::ClientContext context;

            grpc::Status status = stub_->DoEcho(&context, request, &reply);
            if (status.ok()) {
                return reply.message();
            } else {
                return 0;
            }
        }

    private:
        std::unique_ptr<Api::Stub> stub_;
    };
}




int main(int argc, char** argv) {
  rpc::echo::Client client(
      grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
  std::string msg("tangro");
  std::string reply = client.Echo(msg);

  /* Logging */
  std::cout << "[New Echo Reply]: " << reply << std::endl;

  return 0;
}