#pragma once

#include <entt/entt.hpp>

class Event;
class SystemManager;
struct SharedContext;

class System {
    protected:
        SystemManager* m_systemManager;
        SharedContext* m_context;
        entt::registry* m_registry;

    public:
        System(SystemManager* systemManager, SharedContext* context);        

        virtual void init() = 0;

        // dt in milliseconds
        virtual void update(int dt) = 0;
};
