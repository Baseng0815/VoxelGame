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
            World::setBlock(&m_registry, block, Block());
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
    m_registry.view<CameraComponent>().each(
        [&](auto &camera) {
            camera.fov -= e.dy;

            if (camera.fov > 179) {
                camera.fov = 179;
            }
            else if (camera.fov < 1) {
                camera.fov = 1;
            }

            camera.updateProjectionMatrix();
        });
}

void InputSystem::handleFramebufferSizeEvent(const FramebufferSizeEvent &e) {
    entt::entity player = m_registry.view<PlayerComponent>().front();
    CameraComponent &cameraComponent = m_registry.get<CameraComponent>(player);

    cameraComponent.width = e.width;
    cameraComponent.height = e.height;
    cameraComponent.updateProjectionMatrix();
}

void InputSystem::updateVectors(CameraComponent &camera) {
    camera.front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    camera.front.y = sin(glm::radians(camera.pitch));
    camera.front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    camera.front = glm::normalize(camera.front);
    camera.right = glm::normalize(glm::cross(camera.front, glm::vec3(0, 1, 0)));
    camera.front_noY = glm::cross(camera.right, glm::vec3(0, 1, 0));

    camera.viewMatrixOutdated = true;
}

void InputSystem::_update(int dt) {
    entt::entity player = m_registry.view<PlayerComponent>().front();
    CameraComponent &cameraComponent = m_registry.get<CameraComponent>(player);

    if (cameraComponent.viewMatrixOutdated) {
        const TransformationComponent &transformComponent = m_registry.get<TransformationComponent>(player);

        cameraComponent.updateViewMatrix(transformComponent);
    }
}

InputSystem::InputSystem(Registry_T &registry)
    : System{registry, 0} {
    // create camera and update projection matrix
    // TODO put into extra system
    entt::entity entity = m_registry.create();
    m_registry.emplace<CameraComponent>(entity, 90.f, Configuration::getFloatValue("WINDOW_WIDTH"), Configuration::getFloatValue("WINDOW_HEIGHT"));
    m_registry.emplace<TransformationComponent>(entity, glm::vec3{0.f, 100.f, 0.f}, glm::vec3{0.f, 0.f, 0.f}, glm::vec3{1, 1, 1});
    m_registry.emplace<VelocityComponent>(entity);
    m_registry.emplace<PlayerComponent>(entity);
    m_registry.emplace<CollisionComponent>(entity, glm::vec3{-0.5, 0, -0.5}, 1.0f, 2.0f, 1.0f);
    m_registry.emplace<RigidBodyComponent>(entity, 80.f, true);
    m_registry.emplace<InventoryComponent>(entity, 36);

    entt::entity player = m_registry.view<PlayerComponent>().front();
    CameraComponent &cameraComponent = m_registry.get<CameraComponent>(player);
    cameraComponent.updateProjectionMatrix();

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
