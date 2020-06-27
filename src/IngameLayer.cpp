#include "../include/IngameLayer.h"

#include "../include/Systems/InputSystem.h"
#include "../include/Systems/PhysicsSystem.h"
#include "../include/Systems/ChunkCreateSystem.h"
#include "../include/Systems/EntityRenderSystem.h"

#include "../include/Components/AtlasComponent.h"
#include "../include/Components/WorldComponent.h"
#include "../include/Components/CameraComponent.h"
#include "../include/Components/VelocityComponent.h"
#include "../include/Components/RigidBodyComponent.h"
#include "../include/Components/TransformationComponent.h"

#include "../include/Application.h"
#include "../include/ResourceManager.h"
#include "../include/Events/EventDispatcher.h"
#include "../include/Rendering/Texture.h"

IngameLayer::IngameLayer(Application* application)
    : GameLayer(application) {
    m_application->getWindow().disableCursor();

    // create all systems
    m_systems.push_back(new ChunkCreateSystem(&m_registry));
    m_systems.push_back(new PhysicsSystem(&m_registry));
    m_systems.push_back(new InputSystem(&m_registry));
    m_systems.push_back(new EntityRenderSystem(&m_registry));

    // world
    auto entity = m_registry.create();
    m_registry.emplace<WorldComponent>(entity);

    // atlas
    entity = m_registry.create();
    Texture* atlasTexture = ResourceManager::getResource<Texture>("textureAtlas");
    m_registry.emplace<AtlasComponent>(entity, atlasTexture->getWidth(), atlasTexture->getHeight(), 16);

    // raise beginning events
    EnterChunkEvent e;
    e.newX = e.newZ = e.oldX = e.oldZ = 0;
    EventDispatcher::raiseEvent(e);
}

IngameLayer::~IngameLayer() {
    m_registry.view<CameraComponent, RigidBodyComponent>().each(
        [&](CameraComponent& camera, RigidBodyComponent& rigidBody) {
            delete rigidBody.collision;
            delete rigidBody.shape;
        }
    );

    for (auto system : m_systems)
        delete system;
}

void IngameLayer::update(int dt) {
    for (auto system : m_systems)
        system->update(dt);
}
