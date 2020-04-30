#include "../include/SystemManager.h"

#include "../include/ChunkCreateSystem.h"
#include "../include/EntityRenderSystem.h"
#include "../include/InputSystem.h"

#include "../include//EventDispatcher.h"

SystemManager::SystemManager() {
    // create all systems
    m_systems.push_back(new ChunkCreateSystem(this, WorldType::WORLD_FLAT));
    m_systems.push_back(new EntityRenderSystem(this));
    m_systems.push_back(new InputSystem(this));

    // raise beginning events
    EnterChunkEvent e;
    e.newX = e.newZ = e.oldX = e.oldZ = 0;
    EventDispatcher::raiseEvent(&e);
}

SystemManager::~SystemManager() {
    for (auto system : m_systems)
        delete system;
}

void SystemManager::update(int dt) {
    for (auto system : m_systems)
        system->update(dt);
}

entt::registry& SystemManager::getRegistry() {
    return m_entityRegistry;
}

std::mutex& SystemManager::getRegistryMutex() {
    return m_registryMutex;
}
