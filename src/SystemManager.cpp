#include "../include/SystemManager.h"

#include "../include/Texture.h"
#include "../include/InputSystem.h"
#include "../include/ResourceManager.h"
#include "../include/ChunkCreateSystem.h"
#include "../include/EntityRenderSystem.h"

#include "../include//EventDispatcher.h"
#include "../include/Components/AtlasComponent.h"

SystemManager::SystemManager() {
    // TODO move resource loading to a better location
    ResourceManager::loadResources();

    // create all systems
    m_systems.push_back(new ChunkCreateSystem(this));
    m_systems.push_back(new EntityRenderSystem(this));
    m_systems.push_back(new InputSystem(this));

    // create shared entities
    auto entity = m_entityRegistry.create();
    Texture* atlasTexture = ResourceManager::getResource<Texture>("textureAtlas");
    m_entityRegistry.emplace<AtlasComponent>(entity, atlasTexture->getWidth(), atlasTexture->getHeight(), 16);

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
