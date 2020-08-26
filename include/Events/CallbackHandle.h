#include <algorithm>

#include "../Typedefs.h"

template<typename... Args>
class CallbackList;

// automatically unsubscribe on destruction
template<typename... Args>
class CallbackHandle {
    private:
        CallbackList<Args...> *m_list;
        CallbackId m_id;
        bool m_isSubscribed = true;

    public:
        CallbackHandle(CallbackList<Args...> *list, CallbackId id)
            : m_list {list}, m_id {id}
        {}

        CallbackHandle() = default;

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
            : m_list {other.m_list}, m_isSubscribed {other.m_isSubscribed}, m_id {other.m_id}
        {}

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
