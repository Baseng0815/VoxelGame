#include "../include/SystemManager.h"

#include "../include/ChunkCreateSystem.h"
#include "../include/ChunkRenderSystem.h"
#include "../include/InputSystem.h"

void SystemManager::init(SharedContext* context) {
    // create and initialize all systems
    m_systems.push_back(new ChunkCreateSystem(this, context, WorldType::WORLD_FLAT));
    m_systems.push_back(new ChunkRenderSystem(this, context));
    m_systems.push_back(new InputSystem(this, context));
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
