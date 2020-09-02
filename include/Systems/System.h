#pragma once

#include <vector>
#include <entt/entt.hpp>

#include "../Typedefs.h"
#include "../Events/CallbackHandle.h"

using Registry_T = entt::registry;

class System {
    protected:
        Registry_T &m_registry;

        int m_currentTime = 0;
        int m_updateDelay;

        virtual void _update(int dt) = 0;

    public:
        System(Registry_T &registry, int updateDelay);

        // dt in milliseconds
        void update(int dt);
};
