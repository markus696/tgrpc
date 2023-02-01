
#ifndef TGRPC_SERVICE_H
#define TGRPC_SERVICE_H

#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>

#include "rpc_model/echo_service.pb.h"
#include "rpc_model/echo_service.grpc.pb.h"

namespace rpc_services::echo {
    using rpc_model::echo::EchoRequest;
    using rpc_model::echo::EchoReply;
    using rpc_model::echo::Api;

    class RpcService final : public Api::Service {
        grpc::Status DoEcho(grpc::ServerContext * context, const EchoRequest * request, EchoReply * reply) override {
            std::string msg(request->message());

            /* Logging */
            std::cout << "[Echo Request]: " << msg << std::endl;

            reply->set_message(msg);
            return grpc::Status::OK;
        }
    };
}

// Logic and data behind the server's behavior.


#endif //TGRPC_SERVICE_H
