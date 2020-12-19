#pragma once

#include <vector>
#include <entt/entt.hpp>

#include "../Typedefs.hpp"
#include "../Events/CallbackHandle.hpp"

using Registry_T = entt::registry;

class System {
    protected:
        Registry_T &m_registry;

        virtual void _update(int dt) = 0;

    public:
        System(Registry_T &registry);

        // dt in milliseconds
        void update(int dt);
};
