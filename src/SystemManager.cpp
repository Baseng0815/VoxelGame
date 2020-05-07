#include "../include/SystemManager.h"

#include "../include/Texture.h"
#include "../include/ResourceManager.h"
#include "../include//EventDispatcher.h"

#include "../include/InputSystem.h"
#include "../include/PhysicsSystem.h"
#include "../include/ChunkCreateSystem.h"
#include "../include/ChunkCreateSystem.h"
#include "../include/EntityRenderSystem.h"

#include "../include/Components/AtlasComponent.h"
#include "../include/Components/CameraComponent.h"
#include "../include/Components/VelocityComponent.h"
#include "../include/Components/TransformationComponent.h"

SystemManager::SystemManager() {
    // TODO move resource loading to a better location
    ResourceManager::loadResources();

    // create all systems
    m_systems.push_back(new ChunkCreateSystem(this));
    m_systems.push_back(new EntityRenderSystem(this));
    m_systems.push_back(new InputSystem(this));
    m_systems.push_back(new PhysicsSystem(this));

    // atlas
    auto entity = m_entityRegistry.create();
    Texture* atlasTexture = ResourceManager::getResource<Texture>("textureAtlas");
    m_entityRegistry.emplace<AtlasComponent>(entity, atlasTexture->getWidth(), atlasTexture->getHeight(), 16);
    // camera
    entity = m_entityRegistry.create();
    m_entityRegistry.emplace<CameraComponent>(entity, 90.f);
    m_entityRegistry.emplace<TransformationComponent>(entity);
    m_entityRegistry.emplace<VelocityComponent>(entity);

    // raise beginning events
    EnterChunkEvent e;
    e.newX = e.newZ = e.oldX = e.oldZ = 0;
    EventDispatcher::raiseEvent(&e);

    FramebufferSizeEvent event = FramebufferSizeEvent(nullptr, 800, 600);
    EventDispatcher::raiseEvent(&event);
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
