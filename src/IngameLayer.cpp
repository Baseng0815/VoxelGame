#include "../include/IngameLayer.h"

#include "../include/Systems/InputSystem.h"
#include "../include/Systems/PhysicsSystem.h"
#include "../include/Systems/PlayerMovementSystem.h"
#include "../include/Systems/ChunkCreateSystem.h"
#include "../include/Systems/CollisionSystem.h"
#include "../include/Systems/SkyboxSystem.h"
#include "../include/Systems/CloudSystem.h"
#include "../include/Systems/MeshRenderSystem.h"
#include "../include/Systems/DebugRenderSystem.h"

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

void IngameLayer::handleKeys(const KeyEvent &e)
{
    if (e.action == GLFW_PRESS) {
        // handle constexpr keys
        switch (e.key) {

            // handle dynamically bound keys
            default:
                if (e.key == Configuration::getAssociatedKey("KEYBIND_TOGGLE_DEBUG")) {
                    UiProperties &properties = m_gui.getWidget<DebugLayout>("layout_debugpanel").properties();
                    properties.isVisible = !properties.isVisible;
                }
                break;
        }
    }
}

IngameLayer::IngameLayer(Application* application)
    : GameLayer {application}
{
    m_application->getWindow().disableCursor();

    // create all systems
    m_systems.emplace_back(new ChunkCreateSystem {m_registry});
    m_systems.emplace_back(new PhysicsSystem {m_registry});
    m_systems.emplace_back(new InputSystem {m_registry});
    m_systems.emplace_back(new PlayerMovementSystem {m_registry });
    m_systems.emplace_back(new CloudSystem {m_registry });
    m_systems.emplace_back(new SkyboxSystem {m_registry });
    m_systems.emplace_back(new CollisionSystem {m_registry});
    m_systems.emplace_back(new MeshRenderSystem {m_registry});
    m_systems.emplace_back(new DebugRenderSystem {m_registry});


    // callbacks
    m_keyEventHandle = EventDispatcher::onKeyPress.subscribe([&](const KeyEvent &e) {
        handleKeys(e);
    });

    EventDispatcher::raiseEvent(EnterChunkEvent {nullptr, 0, 0, 0, 0});
}

IngameLayer::~IngameLayer() {
    /*m_registry.view<CameraComponent, RigidBodyComponent>().each(
      [&](CameraComponent& camera, RigidBodyComponent& rigidBody) {
      delete rigidBody.collision;
      delete rigidBody.shape;
      }
      );*/
}

void IngameLayer::update(int dt) {
    // reset shared uniform state of all shaders
    for (int resId = SHADER_MIN + 1; resId < SHADER_MAX; resId++) {
        ResourceManager::getResource<Shader>(resId)->setUniformState(false);
    }

    for (auto &system : m_systems) {
        system->update(dt);
    }

    m_time += dt;
    if (m_time > 1000) {
        entt::entity entity = m_registry.view<TransformationComponent, CameraComponent>().front();

        const TransformationComponent &transform = m_registry.get<TransformationComponent>(entity);
        const CameraComponent &camera = m_registry.get<CameraComponent>(entity);

        DebugLayoutInfo info{m_frameCounter * 1000 / m_time,
            m_time * 1000 / std::max(m_frameCounter, 1),
            static_cast<const ChunkCreateSystem*>(m_systems[0].get())->getActiveChunkCount(),
            transform.getPosition(),
            camera.front,
            camera.yaw,
            camera.pitch,
            camera.fov,
            (int)floor(transform.getPosition().x / Configuration::CHUNK_SIZE),
            (int)floor(transform.getPosition().z / Configuration::CHUNK_SIZE)};

        m_gui.getWidget<DebugLayout>("layout_debugpanel").setValues(info);
        m_time = 1;
        m_frameCounter = 0;
    }

    m_gui.update();
    m_gui.draw();

    m_frameCounter++;
}
