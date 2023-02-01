#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <stdio.h>

#include <grpc++/grpc++.h>

#include "rpc_services/echo/service.h"
#include "rpc_services/tg/service.h"

#include "telegram/handlers_cache.h"
#include "telegram/response_reciver.h"
#include "telegram/request_sender.h"

int main(int argc, char ** argv) {
    std::vector<std::thread> threads;

    auto tgClientManager = std::make_shared<td::ClientManager>();
    auto tgHandlersCache = std::make_shared<telegram::HandlersCache>();
    auto tgResponseReceiver =
            std::make_shared<telegram::ResponseReceiver>(tgClientManager, tgHandlersCache);
    auto tgRequestSender =
            std::make_shared<telegram::RequestSender>(tgClientManager, tgHandlersCache);

    threads.emplace_back([tgResponseReceiver] {
        td::ClientManager::execute(td::td_api::make_object<td::td_api::setLogVerbosityLevel>(1));
        tgResponseReceiver->Loop();
    });

    threads.emplace_back([tgRequestSender] {
        std::string server_address("0.0.0.0:50051");

        grpc::ServerBuilder builder;
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

        auto tgService = std::make_shared<rpc_services::tg::RpcService>(tgRequestSender);
        auto echoService = std::make_shared<rpc_services::echo::RpcService>();

        builder.RegisterService(echoService.get());
        builder.RegisterService(tgService.get());
        // Finally assemble the server.
        std::shared_ptr<grpc::Server> server(builder.BuildAndStart());
        server->Wait();
    });

    for (auto& thread: threads) {
        thread.join();
    }

    return 0;
}