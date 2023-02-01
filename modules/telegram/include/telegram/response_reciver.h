#ifndef TGRPC_RESPONSE_RECIVER_H
#define TGRPC_RESPONSE_RECIVER_H

#include <iostream>
#include <unordered_map>

#include <td/telegram/Client.h>

#include "handlers_cache.h"

namespace telegram {
    struct ResponseReceiver {
        static constexpr double WAIT_TIMEOUT = 10.0;

        ResponseReceiver(std::shared_ptr<td::ClientManager> clientManager, std::shared_ptr<HandlersCache> handlersCache) :
            clientManager_(std::move(clientManager)), handlersCache_(std::move(handlersCache)) {}


        void Stop(); //TODO

        void Loop()
        {
            while (true) {
                auto response = clientManager_->receive(WAIT_TIMEOUT);
                if (response.object == nullptr) {
                    continue;
                }

                /* TODO: Logging */
                std::cout
                    << "Tg Received: ClientId=" << response.client_id
                    << " RequestId=" << response.request_id << std::endl << std::endl;

                auto handler = handlersCache_->pop(
                        std::tie(response.client_id, response.request_id));
                handler(std::move(response.object));
            }
        }

    private:
        std::shared_ptr<td::ClientManager> clientManager_;
        std::shared_ptr<HandlersCache> handlersCache_;
    };
}

#endif //TGRPC_RESPONSE_RECIVER_H
