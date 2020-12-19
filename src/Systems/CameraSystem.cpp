#include "../../include/Systems/CameraSystem.hpp"

#include "../../include/World.hpp"
#include "../../include/Gui/GUI.hpp"
#include "../../include/Configuration.hpp"
#include "../../include/Gui/HotbarLayout.hpp"
#include "../../include/Gui/InventoryLayout.hpp"
#include "../../include/Events/EventDispatcher.hpp"

#include "../../include/Components/CameraComponent.hpp"
#include "../../include/Components/ChunkComponent.hpp"
#include "../../include/Components/CollisionComponent.hpp"
#include "../../include/Components/InventoryComponent.hpp"
#include "../../include/Components/PlayerComponent.hpp"
#include "../../include/Components/RigidBodyComponent.hpp"
#include "../../include/Components/TransformationComponent.hpp"
#include "../../include/Components/VelocityComponent.hpp"

void CameraSystem::handleMouseMoveEvent(const CursorEvent& e) {
    // UI
    // TODO add abstract top layout that disables camera events

    // camera movement
    entt::entity player = m_registry.view<PlayerComponent>().front();
    const PlayerComponent& playerComponent = m_registry.get<PlayerComponent>(player);
    if (!playerComponent.inputEnabled)
        return;

    CameraComponent& cameraComponent = m_registry.get<CameraComponent>(player);

    cameraComponent.yaw += e.dx * Configuration::getFloatValue("MOUSE_SENSITIVITY");
    cameraComponent.yaw = std::fmod(cameraComponent.yaw, 360.f);
    cameraComponent.pitch -= e.dy * Configuration::getFloatValue("MOUSE_SENSITIVITY");

    if (cameraComponent.pitch > 89.99) {
        cameraComponent.pitch = 89.99;
    }
    else if (cameraComponent.pitch < -89.99) {
        cameraComponent.pitch = -89.99;
    }

    updateVectors(cameraComponent);
}

void CameraSystem::handleFramebufferSizeEvent(const FramebufferSizeEvent& e) {
    CameraComponent& cameraComponent = m_registry.get<CameraComponent>(m_player);

    cameraComponent.width = e.width;
    cameraComponent.height = e.height;
    updateProjectionMatrix(cameraComponent);
}

void CameraSystem::updateVectors(CameraComponent& camera) {
    camera.front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    camera.front.y = sin(glm::radians(camera.pitch));
    camera.front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    camera.front = glm::normalize(camera.front);
    camera.right = glm::normalize(glm::cross(camera.front, glm::vec3{0.f, 1.f, 0.f}));
    camera.front_noY = glm::cross(camera.right, glm::vec3{0.f, 1.f, 0.f});

    camera.viewMatrixOutdated = true;
}

void CameraSystem::updateViewMatrix(CameraComponent& camera) {
    const TransformationComponent& transform = m_registry.get<TransformationComponent>(m_player);

    camera.viewMatrix = glm::lookAt(transform.getPosition() + camera.positionOffset,
                                    transform.getPosition() + camera.positionOffset + camera.front,
                                    glm::vec3{0.f, 1.f, 0.f});
}

void CameraSystem::updateProjectionMatrix(CameraComponent& camera) {
    camera.perspectiveProjection = glm::perspective(glm::radians(camera.fov), camera.width / (float)camera.height, 0.1f, 7000.f);
}

void CameraSystem::_update(int dt) {
    CameraComponent& camera = m_registry.get<CameraComponent>(m_player);

    if (camera.viewMatrixOutdated) {
        updateViewMatrix(camera);
    }
}

CameraSystem::CameraSystem(Registry_T& registry)
    : System {registry}, m_player {m_registry.view<PlayerComponent>().front()}
{
    // create camera and update projection matrix
    CameraComponent& cameraComponent = m_registry.emplace<CameraComponent>(m_player, 90.f, Configuration::getFloatValue("WINDOW_WIDTH"), Configuration::getFloatValue("WINDOW_HEIGHT"));

    updateVectors(cameraComponent);
    updateViewMatrix(cameraComponent);
    updateProjectionMatrix(cameraComponent);

    // subscribe to events
    m_cursorHandle = EventDispatcher::onCursorMove.subscribe([this](const CursorEvent& e) {
        handleMouseMoveEvent(e);
    });

    m_framebufferHandle = EventDispatcher::onFramebufferSize.subscribe([this](const FramebufferSizeEvent& e) {
        handleFramebufferSizeEvent(e);
    });
}
