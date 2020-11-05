#include "../../include/Systems/InputSystem.hpp"

#include "../../include/Events/EventDispatcher.hpp"
#include "../../include/World.hpp"

#include "../../include/Components/CameraComponent.hpp"
#include "../../include/Components/ChunkComponent.hpp"
#include "../../include/Components/CollisionComponent.hpp"
#include "../../include/Components/PlayerComponent.hpp"
#include "../../include/Components/RigidBodyComponent.hpp"
#include "../../include/Components/TransformationComponent.hpp"
#include "../../include/Components/VelocityComponent.hpp"
#include "../../include/Components/InventoryComponent.hpp"


void InputSystem::handleKeyPressEvent(const KeyEvent &e) {
    entt::entity player = m_registry.view<PlayerComponent>().front();
    PlayerComponent &playerComponent = m_registry.get<PlayerComponent>(player);
    if(!playerComponent.inputEnabled)
        return;

    bool changeFlyMode = false;

    byte inputCode = 0;
    switch (e.key) {
        case GLFW_KEY_W:
            inputCode = 1;
            break;
        case GLFW_KEY_S:
            inputCode = 2;
            break;
        case GLFW_KEY_A:
            inputCode = 4;
            break;
        case GLFW_KEY_D:
            inputCode = 8;
            break;
        case GLFW_KEY_SPACE:
            inputCode = 16;

            // if (e.action == GLFW_RELEASE)
            //     lastSpaceInput = glfwGetTime();
            // else if (e.action == GLFW_PRESS) {
            //     double dt = glfwGetTime() - lastSpaceInput;
            //     if (dt > 0.5) {
            //         changeFlyMode = true;
            //     }
            // }
            break;
        case GLFW_KEY_LEFT_SHIFT:
            inputCode = 32;
            break;
    }

    if (e.action == GLFW_PRESS) {
        playerInputState |= inputCode;
    }
    else if (e.action == GLFW_RELEASE) {
        playerInputState &= ~inputCode;
    }

    float xAxisInput = (playerInputState & 1) - ((playerInputState & 2) >> 1);
    float yAxisInput = ((playerInputState & 4) >> 2) - ((playerInputState & 8) >> 3);
    float zAxisInput = ((playerInputState & 16) >> 4) - ((playerInputState & 32) >> 5);

    playerComponent.xAxisInput = -xAxisInput;
    playerComponent.yAxisInput = -yAxisInput;
    playerComponent.zAxisInput = zAxisInput;
}

void InputSystem::handleMouseButtonEvent(const MouseButtonEvent &e) {
    if (e.button == GLFW_MOUSE_BUTTON_LEFT && e.action == GLFW_RELEASE) {
        entt::entity player = m_registry.view<PlayerComponent>().front();
        const PlayerComponent &playerComponent = m_registry.get<PlayerComponent>(player);
        const glm::vec3 &block = playerComponent.lookAt;

        if (!playerComponent.inputEnabled)
            return;

        // TODO: Fix this
        if (block.y > 0 && block.y < Configuration::CHUNK_HEIGHT) {
            World::setBlock(m_registry, block, Block());
        }
    }
}

void InputSystem::handleMouseMoveEvent(const CursorEvent &e) {
    entt::entity player = m_registry.view<PlayerComponent>().front();
    const PlayerComponent &playerComponent = m_registry.get<PlayerComponent>(player);
    if (!playerComponent.inputEnabled)
        return;

    CameraComponent &cameraComponent = m_registry.get<CameraComponent>(player);

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

void InputSystem::handleScrollEvent(const ScrollEvent &e) {
    CameraComponent &cameraComponent = m_registry.get<CameraComponent>(m_player);
    cameraComponent.fov -= e.dy;

    if (cameraComponent.fov > 179) {
        cameraComponent.fov = 179;
    }
    else if (cameraComponent.fov < 1) {
        cameraComponent.fov = 1;
    }

    updateProjectionMatrix(cameraComponent);
}

void InputSystem::handleFramebufferSizeEvent(const FramebufferSizeEvent &e) {
    CameraComponent &cameraComponent = m_registry.get<CameraComponent>(m_player);

    cameraComponent.width = e.width;
    cameraComponent.height = e.height;
    updateProjectionMatrix(cameraComponent);
}

void InputSystem::updateVectors(CameraComponent &camera) {
    camera.front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    camera.front.y = sin(glm::radians(camera.pitch));
    camera.front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    camera.front = glm::normalize(camera.front);
    camera.right = glm::normalize(glm::cross(camera.front, glm::vec3 {0.f, 1.f, 0.f}));
    camera.front_noY = glm::cross(camera.right, glm::vec3{0.f, 1.f, 0.f});

    camera.viewMatrixOutdated = true;
}

void InputSystem::updateViewMatrix(CameraComponent &camera)
{
    const TransformationComponent &transform = m_registry.get<TransformationComponent>(m_player);

    camera.viewMatrix = glm::lookAt(transform.getPosition() + camera.positionOffset,
                                             transform.getPosition() + camera.positionOffset + camera.front,
                                             glm::vec3 {0.f, 1.f, 0.f});
}

void InputSystem::updateProjectionMatrix(CameraComponent &camera) {
    camera.perspectiveProjection = glm::perspective(glm::radians(camera.fov), camera.width / (float)camera.height, 0.1f, 7000.f);
}

void InputSystem::_update(int dt) {
    CameraComponent &camera = m_registry.get<CameraComponent>(m_player);

    if (camera.viewMatrixOutdated) {
        updateViewMatrix(camera);
    }
}

// TODO maybe create player somewhere else
InputSystem::InputSystem(Registry_T &registry)
    : System{registry, 0}, m_player {m_registry.create()} {
        m_registry.emplace<TransformationComponent>(m_player, glm::vec3{0.f, 100.f, 0.f}, glm::vec3{0.f, 0.f, 0.f}, glm::vec3{1, 1, 1});
        m_registry.emplace<VelocityComponent>(m_player);
        m_registry.emplace<PlayerComponent>(m_player);
        m_registry.emplace<CollisionComponent>(m_player, glm::vec3{-0.5, 0, -0.5}, 1.0f, 2.0f, 1.0f);
        m_registry.emplace<RigidBodyComponent>(m_player, 80.f, true);
        m_registry.emplace<InventoryComponent>(m_player, 36);

        // create camera and update projection matrix
        CameraComponent &cameraComponent = m_registry.emplace<CameraComponent>(m_player, 90.f, Configuration::getFloatValue("WINDOW_WIDTH"), Configuration::getFloatValue("WINDOW_HEIGHT"));

        updateVectors(cameraComponent);
        updateViewMatrix(cameraComponent);
        updateProjectionMatrix(cameraComponent);

        // subscribe to events
        m_keyPressHandle = EventDispatcher::onKeyPress.subscribe([this](const KeyEvent &e) {
            handleKeyPressEvent(e);
        });

        m_mouseButtonHandle = EventDispatcher::onMouseButtonPress.subscribe([this](const MouseButtonEvent &e) {
            handleMouseButtonEvent(e);
        });

        m_cursorHandle = EventDispatcher::onCursorMove.subscribe([this](const CursorEvent &e) {
            handleMouseMoveEvent(e);
        });

        m_scrollHandle = EventDispatcher::onMouseScroll.subscribe([this](const ScrollEvent &e) {
            handleScrollEvent(e);
        });

        m_framebufferHandle = EventDispatcher::onFramebufferSize.subscribe([this](const FramebufferSizeEvent &e) {
            handleFramebufferSizeEvent(e);
        });
    }
