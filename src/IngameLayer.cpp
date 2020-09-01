#include "../include/IngameLayer.h"

#include "../include/Systems/InputSystem.h"
#include "../include/Systems/PhysicsSystem.h"
#include "../include/Systems/ChunkCreateSystem.h"
#include "../include/Systems/MeshRenderSystem.h"

#include "../include/Components/AtlasComponent.h"
#include "../include/Components/WorldComponent.h"
#include "../include/Components/CameraComponent.h"
#include "../include/Components/VelocityComponent.h"
#include "../include/Components/RigidBodyComponent.h"
#include "../include/Components/TransformationComponent.h"

#include "../include/Application.h"
#include "../include/Gui/DebugLayout.h"
#include "../include/Resources/ResourceManager.h"
#include "../include/Resources/Texture.h"
#include "../include/Events/EventDispatcher.h"


IngameLayer::IngameLayer(Application* application)
    : GameLayer(application) {
        m_application->getWindow().disableCursor();

        // create all systems
        m_systems.emplace_back(std::unique_ptr<System> {new ChunkCreateSystem {m_registry}});
        m_systems.emplace_back(std::unique_ptr<System> {new PhysicsSystem {m_registry}});
        m_systems.emplace_back(std::unique_ptr<System> {new InputSystem {m_registry}});
        m_systems.emplace_back(std::unique_ptr<System> {new MeshRenderSystem {m_registry}});

        // world
        auto entity = m_registry.create();
        m_registry.emplace<WorldComponent>(entity);

        // atlas
        entity = m_registry.create();
        const Texture *atlasTexture = ResourceManager::getResource<Texture>(TEXTURE_ATLAS);
        m_registry.emplace<AtlasComponent>(entity, atlasTexture->getWidth(), atlasTexture->getHeight(), 16);

        m_gui.addPanel(new DebugLayout(m_gui));
    }

IngameLayer::~IngameLayer() {
    m_registry.view<CameraComponent, RigidBodyComponent>().each(
        [&](CameraComponent& camera, RigidBodyComponent& rigidBody) {
            delete rigidBody.collision;
            delete rigidBody.shape;
        }
        );
}

void IngameLayer::update(int dt) {
    for (auto &system : m_systems) {
        system->update(dt);
    }

    m_time += dt;
    if (m_time > 1000) {
        m_gui.getWidget<DebugLayout>("layout_debugpanel").setValues(m_frameCounter / (float)m_time * 1000.f,
                                                                    m_time / (float)m_frameCounter * 1000.f, static_cast<const ChunkCreateSystem*>(m_systems[0].get())->getActiveChunkCount());
        m_time = 0;
        m_frameCounter = 0;
    }

    m_gui.update();
    m_gui.draw();

    m_frameCounter++;
}
