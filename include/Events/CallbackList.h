#pragma once

#include <map>
#include <functional>

#include "CallbackHandle.h"

template<typename... Args>
class CallbackList {
    private:
        std::map<CallbackId, std::function<void(Args...)>> m_functions;
        CallbackId m_idCounter = 0;

    public:
        CallbackHandle<Args...> subscribe(std::function<void(Args...)> func) {
            m_functions[m_idCounter] = func;
            return CallbackHandle<Args...> { this, m_idCounter++ };
        }

        void unsubscribe(CallbackId id) {
            m_functions.erase(id);
        }

        void invoke(Args... args) {
            for (auto const &[key, func] : m_functions) {
                func(args...);
            }
        }
};
