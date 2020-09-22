#pragma once

#include <algorithm>

#include "../Typedefs.hpp"

template<typename... Args>
class CallbackList;

// automatically unsubscribe on destruction
template<typename... Args>
class CallbackHandle {
    private:
        CallbackList<Args...> *m_list;
        CallbackId m_id;
        bool m_isSubscribed = false;

    public:
        CallbackHandle() = default;
        CallbackHandle(CallbackList<Args...> *list, CallbackId id)
            : m_list {list}, m_id {id}, m_isSubscribed {true}
        {}

        void unsubscribe()
        {
            if (m_isSubscribed) {
                m_list->unsubscribe(m_id);
                m_isSubscribed = false;
            }
        }

        ~CallbackHandle()
        {
            unsubscribe();
        }

        CallbackHandle(const CallbackHandle&) = delete;
        CallbackHandle& operator=(const CallbackHandle&) = delete;

        CallbackHandle(CallbackHandle &&other)
            : m_list {other.m_list}, m_id {other.m_id}, m_isSubscribed {other.m_isSubscribed}
        {
            other.m_list = nullptr;
            other.m_id = 0;
            other.m_isSubscribed = false;
        }

        CallbackHandle& operator=(CallbackHandle &&other)
        {
            if (this != &other) {
                std::swap(m_list, other.m_list);
                std::swap(m_id, other.m_id);
                std::swap(m_isSubscribed, other.m_isSubscribed);
            }

            return *this;
        }
};
