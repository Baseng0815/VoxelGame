#include "../include/SystemManager.h"

#include "../include/ChunkCreateSystem.h"

void SystemManager::init() {
    // create and initialize all systems
    m_systems.push_back(new ChunkCreateSystem(this, WorldType::WORLD_FLAT));
}

SystemManager::~SystemManager() {
    for (auto system : m_systems)
        delete system;
}

void SystemManager::update(int dt) {
    for (auto system : m_systems)
        system->update(dt);
}

void SystemManager::handleEvent(Event *e) {
    for (auto system : m_systems)
        system->handleEvent(e);
}

entt::registry& SystemManager::getRegistry() {
    return m_entityRegistry;
}
