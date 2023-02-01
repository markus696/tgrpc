#ifndef TGRPC_REQUEST_SENDER_H
#define TGRPC_REQUEST_SENDER_H

#include <iostream>
#include <unordered_map>
#include <memory>
#include <functional>
#include <utility>

#include <td/telegram/Client.h>

#include "handlers_cache.h"

namespace telegram {
    struct RequestSender {
        RequestSender(std::shared_ptr<td::ClientManager> clientManager, std::shared_ptr<HandlersCache> handlersCache) :
                clientManager_(std::move(clientManager)), handlersCache_(std::move(handlersCache)), currentRequestId_(0) {}

        void SendRequest(td::ClientManager::ClientId clientId, td::td_api::object_ptr<td::td_api::Function> request,
                         HandlersCache::Handler&& handler)
        {
            td::ClientManager::RequestId requestId = NextRequestId();
            if (handler) {
                handlersCache_->set(std::tie(clientId, requestId), std::move(handler));
            }
            clientManager_->send(clientId, requestId, std::move(request));
        }

    private:

        td::ClientManager::RequestId NextRequestId()
        {
            return ++currentRequestId_;
        }

        std::shared_ptr<td::ClientManager> clientManager_;
        std::shared_ptr<HandlersCache> handlersCache_;
        td::ClientManager::RequestId currentRequestId_;
    };
}

#endif //TGRPC_REQUEST_SENDER_H
