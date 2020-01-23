#include <vector>
#include <entt/entt.hpp>

class System;
class Event;

class SystemManager {
    private:
        std::vector<System*> m_systems;

        entt::registry m_entityRegistry;

    public:
        void init();
        ~SystemManager();

        void update(int dt);
        void handleEvent(Event* e);

        entt::registry& getRegistry() const;
};
