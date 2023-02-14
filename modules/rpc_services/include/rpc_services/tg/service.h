
#ifndef TGRPC_SERVICE_H
#define TGRPC_SERVICE_H

#include <iostream>
#include <memory>
#include <string>
#include <future>

#include <grpc++/grpc++.h>
#include <quill/Quill.h>

#include "telegram/request_sender.h"
#include "rpc_model/tg_service.pb.h"
#include "rpc_model/tg_service.grpc.pb.h"

namespace rpc_services::tg {
    using rpc_model::tg::OptionRequest;
    using rpc_model::tg::OptionResponse;
    using rpc_model::tg::Error;
    using rpc_model::tg::Api;

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

    struct RpcService final : public Api::Service {
        using TgResponseObject = td::td_api::object_ptr<td::td_api::Object>;

        explicit RpcService(std::shared_ptr<telegram::RequestSender> tgRequestSender) :
                tgRequestSender_(std::move(tgRequestSender)) {}

        grpc::Status GetOption(grpc::ServerContext * context, const  OptionRequest * request, OptionResponse * reply) override {
            std::string optionName = request->name();
            auto tgRequestObj = td::td_api::make_object<td::td_api::getOption>(optionName);

            /* ========================================== TODO =========================================== */
            /* ========================================== TODO =========================================== */
            auto promPtr = std::make_shared<std::promise<void>>();
            auto future = promPtr->get_future();

            tgRequestSender_->SendRequest(1, std::move(tgRequestObj), [promPtr, reply](TgResponseObject obj) {
                td::td_api::downcast_call(*obj, overloaded(
                        [reply](td::td_api::optionValueBoolean& option) {
                            reply->set_bool_val(option.value_);
                        },
                        [reply](td::td_api::optionValueInteger& option) {
                            reply->set_int_val(option.value_);
                        },
                        [reply](td::td_api::optionValueString& option) {
                            reply->set_str_val(option.value_);
                        },
                        [reply](td::td_api::optionValueEmpty& option) {},
                        [reply](td::td_api::error& tgErr) {
                            Error* err = new Error();
                            err->set_code(tgErr.code_);
                            err->set_msg(tgErr.message_);
                            reply->set_allocated_error(err);
                        },
                        [](auto& obj) {}));

                promPtr->set_value();
            });

            future.get();
            /* ========================================== TODO =========================================== */
            /* ========================================== TODO =========================================== */

            return grpc::Status::OK;
        }

    private:
        std::shared_ptr<telegram::RequestSender> tgRequestSender_;
    };
}

// Logic and data behind the server's behavior.


#endif //TGRPC_SERVICE_H
