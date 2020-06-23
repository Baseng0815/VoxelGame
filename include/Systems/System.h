#pragma once

#include <entt/entt.hpp>

#include "../Typedefs.h"

class Event;
struct SharedContext;

class System {
    protected:
        entt::registry* m_registry;

        int m_currentTime = 0;
        int m_updateDelay;
        std::vector<CallbackId> m_callbacks;

        virtual void _update(int dt) = 0;

    public:
        System(entt::registry* registry, int updateDelay);
        ~System();

        // dt in milliseconds
        void update(int dt);
};
