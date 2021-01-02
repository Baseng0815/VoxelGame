#include "../../include/Systems/InputSystem.hpp"

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

void InputSystem::handleKeyPressEvent(const KeyEvent& e) {
    // TODO use m_player instead of retrieving it every time
    entt::entity player = m_registry.view<PlayerComponent>().front();
    PlayerComponent     &playerComponent    = m_registry.get<PlayerComponent>(player);
    RigidBodyComponent  &rigidBody          = m_registry.get<RigidBodyComponent>(player);

    if (!playerComponent.inputEnabled) {
        return;
    }

    // TODO maybe integrate this into the giant block of conditions below
    bool doubleKeyDetected = false;
    int currentTime = glfwGetTime() * 1000;
    if (e.action == GLFW_PRESS) {
        // check for double key presses (change to fly mode)
        std::cout << currentTime - m_lastKeyTime << "\n";
        if (e.key == m_lastKey && (currentTime - m_lastKeyTime) < Configuration::KEY_INTERVAL) {
            // double key press recognized; reset stuff and take double key action
            std::cout << "DOUBLE KEY DETECTED\n";
            doubleKeyDetected = true;
            m_lastKey = GLFW_KEY_UNKNOWN;
            m_lastKeyTime = 0;
            switch (e.key) {
                case GLFW_KEY_SPACE:
                    rigidBody.gravityApplies = !rigidBody.gravityApplies;
                    playerComponent.isFlying = !playerComponent.isFlying;
                    // the first time the player starts flying, y velocity has to be set to 0
                    if (playerComponent.isFlying) {
                        VelocityComponent &velocity = m_registry.get<VelocityComponent>(player);
                        velocity.velocity.y = 0.f;
                    }
                    break;


                default:
                    // act as if it was not pressed at all if it is not handled
                    doubleKeyDetected = false;
                    break;
            }
        } else {
            // no double key press recognized, but key was pressed; update stuff
            m_lastKey = e.key;
            m_lastKeyTime = currentTime;
        }
    }


    byte inputCode = 0;
    if (!doubleKeyDetected) {
        // no double key press recognized; update stuff and take usual key action
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
                break;
            case GLFW_KEY_LEFT_SHIFT:
                inputCode = 32;
                break;
        }

        if (e.action == GLFW_PRESS) {
            playerInputState |= inputCode;
        } else if (e.action == GLFW_RELEASE) {
            playerInputState &= ~inputCode;
        }

        float xAxisInput = (playerInputState & 1) - ((playerInputState & 2) >> 1);
        float yAxisInput = ((playerInputState & 4) >> 2) - ((playerInputState & 8) >> 3);
        float zAxisInput = ((playerInputState & 16) >> 4) - ((playerInputState & 32) >> 5);

        playerComponent.xAxisInput = -xAxisInput;
        playerComponent.yAxisInput = -yAxisInput;
        playerComponent.zAxisInput = zAxisInput;
    }
}

void InputSystem::handleMouseButtonEvent(const MouseButtonEvent& e) {
    if (e.action == GLFW_RELEASE) {
        if (e.button == GLFW_MOUSE_BUTTON_LEFT) {
            entt::entity player = m_registry.view<PlayerComponent>().front();
            const PlayerComponent& playerComponent = m_registry.get<PlayerComponent>(player);
            const glm::vec3& block = playerComponent.lookAt;

            if (!playerComponent.inputEnabled)
                return;

            if (block.y > 0 && block.y < Configuration::CHUNK_HEIGHT) {
                World::setBlock(m_registry, Block{block, BlockId::BLOCK_AIR});
            }
        }
        else if (e.button == GLFW_MOUSE_BUTTON_RIGHT) {
            entt::entity player = m_registry.view<PlayerComponent>().front();
            const PlayerComponent& playerComponent = m_registry.get<PlayerComponent>(player);
            const glm::vec3& block = playerComponent.lookAt;
            const CameraComponent& camera = m_registry.get<CameraComponent>(player);
            const TransformationComponent& transform = m_registry.get<TransformationComponent>(player);

            const Math::Ray& lookDir = Math::Ray{transform.getPosition() + camera.positionOffset, camera.front};

            const glm::vec3 &faceNormal = lookDir.getIntersectionFaceNormal(block);
            std::cout << "face normal: " << faceNormal << std::endl;
            if (glm::length2(faceNormal) > 0) {
                InventoryComponent& playerInventory = m_registry.get<InventoryComponent>(player);
                const auto& [type, count] = playerInventory.slots[playerComponent.selectedItemIndex];
                if (count > 0) {
                    World::setBlock(m_registry, Block{block + faceNormal, type});

                    playerInventory.slots[playerComponent.selectedItemIndex].second--;
                }
            }
        }
    }
}

void InputSystem::handleScrollEvent(const ScrollEvent& e) {
    // UI hotbar scroll
    int currentIndex = m_hotbarLayout.getSelectionIndex();
    int index = (currentIndex + (e.dy < 0 ? 1 : 8)) % 9;
    m_hotbarLayout.setSelectionIndex(index);

    m_registry.view<PlayerComponent>().raw()[0].selectedItemIndex = index;
}

void InputSystem::_update(int dt)
{  }

// TODO maybe create player somewhere else
InputSystem::InputSystem(Registry_T& registry, InventoryLayout &inventoryLayout, HotbarLayout &hotbarLayout)
    : System {registry}, m_player {m_registry.create()}, m_lastKey {GLFW_KEY_UNKNOWN},
    m_inventoryLayout {inventoryLayout}, m_hotbarLayout {hotbarLayout}
{
    m_registry.emplace<TransformationComponent>(m_player, glm::vec3{0.f, 100.f, 0.f}, glm::vec3{0.f, 0.f, 0.f}, glm::vec3{1, 1, 1});
    m_registry.emplace<VelocityComponent>(m_player);
    m_registry.emplace<PlayerComponent>(m_player);
    m_registry.emplace<CollisionComponent>(m_player, glm::vec3{-0.5, 0, -0.5}, 1.0f, 2.0f, 1.0f);
    m_registry.emplace<RigidBodyComponent>(m_player, 80.f, true);
    m_registry.emplace<InventoryComponent>(m_player, 36);

    // subscribe to events
    m_keyPressHandle = EventDispatcher::onKeyPress.subscribe([this](const KeyEvent& e) {
        handleKeyPressEvent(e);
    });

    m_mouseButtonHandle = EventDispatcher::onMouseButtonPress.subscribe([this](const MouseButtonEvent& e) {
        handleMouseButtonEvent(e);
    });

    m_scrollHandle = EventDispatcher::onMouseScroll.subscribe([this](const ScrollEvent& e) {
        handleScrollEvent(e);
    });
}
