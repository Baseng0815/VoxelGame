#pragma once

#include <vector>
#include <mutex>
#include <entt/entt.hpp>
#include <atomic>

class System;
class Event;
struct SharedContext;

class SystemManager {
private:
    std::vector<System*> m_systems;

    std::mutex m_registryMutex;
    entt::registry m_entityRegistry;

public:
    std::atomic_bool physicsActive = false;

    SystemManager();
    ~SystemManager();

    void update(int dt);

    entt::registry& getRegistry();
    std::mutex& getRegistryMutex();
};
