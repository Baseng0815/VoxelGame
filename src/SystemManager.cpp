#include "../include/SystemManager.h"

#include "../include/ChunkCreateSystem.h"
#include "../include/ChunkRenderSystem.h"
#include "../include/InputSystem.h"

#include "../include//EventDispatcher.h"

void SystemManager::init(SharedContext* context) {
    // create all systems
    m_systems.push_back(new ChunkCreateSystem(this, context, WorldType::WORLD_FLAT));
    m_systems.push_back(new ChunkRenderSystem(this, context));
    m_systems.push_back(new InputSystem(this, context));

    // initialize all systems (two-stage initialization in case any dependencies arise, which really shouldn't, but who knows?)
    for (auto& system : m_systems) {
        system->init();
    }

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
