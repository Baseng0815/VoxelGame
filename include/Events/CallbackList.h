#pragma once

#include <map>
#include <functional>

#include "../Typedefs.h"

typedef unsigned int CallbackId;

template<typename... Args>
class CallbackList {
    private:
        std::map<CallbackId, std::function<void(Args...)>> m_functions;
        CallbackId m_idCounter = 0;

    public:
        CallbackId operator+=(std::function<void(Args...)> func) {
            m_functions[m_idCounter] = func;
            return m_idCounter++;
        }

        void operator-=(CallbackId id) {
            m_functions.erase(id);
        }

        void invoke(Args... args) {
            for (auto const& [key, func] : m_functions) {
                func(args...);
            }
        }
};
