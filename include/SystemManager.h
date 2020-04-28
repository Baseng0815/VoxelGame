#pragma once

#include <vector>
#include <mutex>
#include <entt/entt.hpp>

class System;
class Event;
struct SharedContext;

class SystemManager {
private:
    std::vector<System*> m_systems;

    std::mutex m_registryMutex;
    entt::registry m_entityRegistry;

public:
    SystemManager(SharedContext* context);
    ~SystemManager();

    void update(int dt);

    entt::registry& getRegistry();
    std::mutex& getRegistryMutex();
};
