#pragma once

#include <entt/entt.hpp>

class Event;
class SystemManager;

class System {
    protected:
        SystemManager* m_systemManager;
        entt::registry* m_registry;

    public:
        System(SystemManager* systemManager);

        virtual void init() = 0;

        // dt in milliseconds
        virtual void update(int dt) = 0;
};
