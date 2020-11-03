#include "../include/IngameLayer.hpp"

#include "../include/Systems/ChunkCreateSystem.hpp"
#include "../include/Systems/CloudSystem.hpp"
#include "../include/Systems/CollisionSystem.hpp"
#include "../include/Systems/DebugRenderSystem.hpp"
#include "../include/Systems/InputSystem.hpp"
#include "../include/Systems/ItemSystem.hpp"
#include "../include/Systems/MeshRenderSystem.hpp"
#include "../include/Systems/PhysicsSystem.hpp"
#include "../include/Systems/PlayerMovementSystem.hpp"
#include "../include/Systems/SkyboxSystem.hpp"
#include "../include/Systems/ChunkUpdateSystem.hpp"

#include "../include/Components/CameraComponent.hpp"
#include "../include/Components/InventoryComponent.hpp"
#include "../include/Components/PlayerComponent.hpp"
#include "../include/Components/RigidBodyComponent.hpp"
#include "../include/Components/TransformationComponent.hpp"
#include "../include/Components/VelocityComponent.hpp"

#include "../include/Application.hpp"
#include "../include/Events/EventDispatcher.hpp"
#include "../include/Gui/DebugLayout.hpp"
#include "../include/Gui/Image.hpp"
#include "../include/Gui/InventoryLayout.hpp"
#include "../include/Resources/ResourceManager.hpp"
#include "../include/Resources/Texture.hpp"

void IngameLayer::handleKeys(const KeyEvent &e) {
    if (e.action == GLFW_PRESS) {
        // handle constexpr keys
        switch (e.key) {
        // handle dynamically bound keys
        default:
            if (e.key == Configuration::getAssociatedKey("KEYBIND_TOGGLE_DEBUG")) {
                UiProperties &properties = m_gui.getWidget<DebugLayout>("layout_debugpanel").properties();
                properties.isVisible = !properties.isVisible;
            }
            else if (e.key == Configuration::getAssociatedKey("KEYBIND_OPEN_INVENTORY")) {
                InventoryLayout &layout = m_gui.getWidget<InventoryLayout>("layout_inventory");

                UiProperties &inventory = layout.properties();
                UiProperties &crosshair = m_gui.getWidget<Image>("image_crosshair").properties();
                entt::entity player = m_registry.view<PlayerComponent>().front();
                PlayerComponent &playerComponent = m_registry.get<PlayerComponent>(player);

                inventory.isVisible = !inventory.isVisible;
                crosshair.isVisible = !inventory.isVisible;
                playerComponent.inputEnabled = !inventory.isVisible;

                if (inventory.isVisible) {
                    m_application->getWindow().enableCursor();
                    layout.setInventory(m_registry.get<InventoryComponent>(player));
                }
                else {
                    m_application->getWindow().disableCursor();
                }
            }
            break;
        }
    }
}

IngameLayer::IngameLayer(Application *application)
    : GameLayer{application}, m_atlas{16, 16}
{
    m_application->getWindow().disableCursor();

    // create all systems
    // updating and physics
    m_systems.emplace_back(new ChunkCreateSystem{m_registry, m_atlas});
    m_systems.emplace_back(new PhysicsSystem{m_registry});
    m_systems.emplace_back(new InputSystem{m_registry});
    m_systems.emplace_back(new PlayerMovementSystem{m_registry});
    m_systems.emplace_back(new CloudSystem{m_registry});
    m_systems.emplace_back(new SkyboxSystem{m_registry});
    m_systems.emplace_back(new CollisionSystem{m_registry});
    m_systems.emplace_back(new ItemSystem{m_registry, m_atlas});
    m_systems.emplace_back(new ChunkUpdateSystem{m_registry});

    // rendering
    m_systems.emplace_back(new MeshRenderSystem{m_registry});
    m_systems.emplace_back(new DebugRenderSystem{m_registry});

    // callbacks
    m_keyEventHandle = EventDispatcher::onKeyPress.subscribe([&](const KeyEvent &e) {
        handleKeys(e);
    });

    EventDispatcher::raiseEvent(EnterChunkEvent{nullptr, 0, 0, 0, 0});
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
                             static_cast<const ChunkCreateSystem *>(m_systems[0].get())->getActiveChunkCount(),
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
