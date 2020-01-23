#include <entt/entt.hpp>

class Event;
class SystemManager;

class System {
    protected:
        SystemManager* m_systemManager;

    public:
        System(SystemManager* systemManager);

        // dt in milliseconds
        virtual void update(int dt) = 0;
        virtual void handleEvent(Event* e) = 0;
}
