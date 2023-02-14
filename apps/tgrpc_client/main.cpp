#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>

#include "rpc_model/tg_service.pb.h"
#include "rpc_model/tg_service.grpc.pb.h"

namespace rpc::tg {
    using rpc_model::tg::OptionRequest;
    using rpc_model::tg::OptionResponse;
    using rpc_model::tg::Error;
    using rpc_model::tg::Api;

    class Client {
    public:
        Client(std::shared_ptr<grpc::Channel> channel)
        : stub_(rpc_model::tg::Api::NewStub(channel)) {}

        OptionResponse GetOption(const std::string& optionName) {
            OptionRequest request;
            request.set_name(optionName);
            OptionResponse reply;
            grpc::ClientContext context;

            grpc::Status status = stub_->GetOption(&context, request, &reply);
            if (status.ok()) {
                return reply;
            } else {
                throw 1;
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

  auto optionResponse = client.GetOption(optionName);
  if (optionResponse.has_error()) {
      std::cout << "[Error] CODE=" << optionResponse.error().code() << " MESSAGE=" << optionResponse.error().msg() << std::endl;
  }
  switch(optionResponse.option_case()) {
      case rpc_model::tg::OptionResponse::OptionCase::kBoolVal:
          std::cout << "Option version: " << optionResponse.bool_val() << std::endl;
          break;
      case rpc_model::tg::OptionResponse::OptionCase::kIntVal:
          std::cout << "Option version: " << optionResponse.int_val() << std::endl;
          break;
      case rpc_model::tg::OptionResponse::OptionCase::kStrVal:
          std::cout << "Option version: " << optionResponse.str_val() << std::endl;
          break;
      case rpc_model::tg::OptionResponse::OptionCase::OPTION_NOT_SET:
          std::cout << "Option version: empty" << std::endl;
          break;
  }

  return 0;
}