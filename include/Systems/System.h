#pragma once

#include <entt/entt.hpp>

#include "../Callback.h"

class Event;
class SystemManager;
struct SharedContext;

class System {
    protected:
        SystemManager* m_systemManager;
        int m_currentTime = 0;
        int m_updateDelay;
        std::vector<CallbackId> m_callbacks;

        virtual void _update(int dt) = 0;

    public:
        System(SystemManager* systemManager, int updateDelay);

        // dt in milliseconds
        void update(int dt);

        void detachEvents() const;
        void attachEvents() const;
};
