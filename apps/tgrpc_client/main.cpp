#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>

#include "rpc_model/tg_service.pb.h"
#include "rpc_model/tg_service.grpc.pb.h"

namespace rpc::tg {
    using rpc_model::tg::OptionRequest;
    using rpc_model::tg::Option;
    using rpc_model::tg::Api;

    class Client {
    public:
        Client(std::shared_ptr<grpc::Channel> channel)
        : stub_(rpc_model::tg::Api::NewStub(channel)) {}

        void GetOption(const std::string& optionName) {
            OptionRequest request;
            request.set_name(optionName);
            Option reply;
            grpc::ClientContext context;

            grpc::Status status = stub_->GetOption(&context, request, &reply);
            if (status.ok()) {
                std::cout << "option readed" << std::endl;
            } else {
                return;
            }
        }

    private:
        std::unique_ptr<Api::Stub> stub_;
    };
}




int main(int argc, char** argv) {
  rpc::tg::Client client(
      grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
  std::string optionName("version");
  client.GetOption(optionName);

  return 0;
}