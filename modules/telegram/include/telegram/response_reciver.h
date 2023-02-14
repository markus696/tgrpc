#ifndef TGRPC_RESPONSE_RECIVER_H
#define TGRPC_RESPONSE_RECIVER_H

#include <iostream>
#include <unordered_map>

#include <td/telegram/Client.h>

#include "handlers_cache.h"

namespace telegram {
    namespace detail {
        template <class... Fs>
        struct overload;

        template <class F>
        struct overload<F> : public F {
            explicit overload(F f) : F(f) {
            }
        };
        template <class F, class... Fs>
        struct overload<F, Fs...>
                : public overload<F>
                        , public overload<Fs...> {
            overload(F f, Fs... fs) : overload<F>(f), overload<Fs...>(fs...) {
            }
            using overload<F>::operator();
            using overload<Fs...>::operator();
        };
    }

    template <class... F>
    auto overloaded(F... f) {
        return detail::overload<F...>(f...);
    }

    struct ResponseReceiver {
        static constexpr double WAIT_TIMEOUT = 10.0;

        ResponseReceiver(std::shared_ptr<td::ClientManager> clientManager, std::shared_ptr<HandlersCache> handlersCache) :
            clientManager_(std::move(clientManager)), handlersCache_(std::move(handlersCache)) {}


        void Stop(); //TODO

        void Loop()
        {
            while (true) {
                auto response = clientManager_->receive(WAIT_TIMEOUT);
                auto obj = std::move(response.object);
                if (obj == nullptr) {
                    continue;
                }

                LOG_INFO(quill::get_logger(),
                         "Received(clientId={}, requestId={})", response.client_id, response.request_id);


                auto on_error_handler = [](td::td_api::error& tgErr) {
                    LOG_ERROR(quill::get_logger(),
                              "TgError(code={}, message={})", tgErr.code_, tgErr.message_);
                };

                td::td_api::downcast_call(*obj, overloaded(on_error_handler, [](auto& obj) {}));

                auto handler = handlersCache_->pop(
                        std::tie(response.client_id, response.request_id));
                handler(std::move(obj));
            }
        }

    private:
        std::shared_ptr<td::ClientManager> clientManager_;
        std::shared_ptr<HandlersCache> handlersCache_;
    };
}

#endif //TGRPC_RESPONSE_RECIVER_H
