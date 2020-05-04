#pragma once

#include <entt/entt.hpp>

class Event;
class SystemManager;
struct SharedContext;

class System {
    protected:
        SystemManager* m_systemManager;
        int m_currentTime = 0;
        int m_updateDelay;

        virtual void _update(int dt) = 0;

    public:
        System(SystemManager* systemManager, int updateDelay);

        // dt in milliseconds
        void update(int dt);
};
