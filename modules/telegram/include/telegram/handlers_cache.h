
#ifndef TGRPC_HANDLERS_CACHE_H
#define TGRPC_HANDLERS_CACHE_H

#include <unordered_map>
#include <memory>
#include <functional>
#include <tuple>
#include <boost/functional/hash.hpp>

#include <td/telegram/Client.h>

namespace std
{
    template<typename... T>
    struct hash<tuple<T...>>
    {
        size_t operator()(tuple<T...> const& arg) const noexcept
        {
            return boost::hash_value(arg);
        }
    };
}

namespace telegram {
    struct HandlersCache {
        using HandlerReturn = void;
        using HandlerArgOne = td::td_api::object_ptr<td::td_api::Object>;
        using Handler = std::function<HandlerReturn(HandlerArgOne)>;
        using UniqueValue = std::tuple<td::ClientManager::ClientId, td::ClientManager::RequestId>;

        void set(UniqueValue uniqueVal, Handler&& handler)
        {
            cache_[uniqueVal] = handler;
        }

        void remove(UniqueValue uniqueVal)
        {
            auto handler = cache_.find(uniqueVal);
            if (handler != cache_.end()) {
                cache_.erase(handler);
            }
        }

        Handler pop(UniqueValue uniqueVal)
        {
            auto handler = cache_.find(uniqueVal);
            if (handler == cache_.end()) {
                throw 1;
            }

            auto res = handler->second;
            return res;
        }

    private:
        std::unordered_map<UniqueValue, Handler> cache_;
    };
}

#endif //TGRPC_HANDLERS_CACHE_H
