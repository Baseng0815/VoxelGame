#include "../../include/Systems/InputSystem.h"

#include "../../include/Block.h"
#include "../../include/Collision.h"
#include "../../include/Configuration.h"
#include "../../include/EventDispatcher.h"
#include "../../include/Systems/SystemManager.h"

#include "../../include/Components/ChunkComponent.h"
#include "../../include/Components/WorldComponent.h"
#include "../../include/Components/CameraComponent.h"
#include "../../include/Components/VelocityComponent.h"
#include "../../include/Components/TransformationComponent.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


void InputSystem::handleKeyPressEvent(Event* e) {
    KeyEvent keyEvent = *e->get<KeyEvent>();

    m_systemManager->getRegistry().view<CameraComponent, VelocityComponent, TransformationComponent>().each(
            [&](auto& camera, auto& velocity, auto& transformation) {
            switch (keyEvent.key) {
                case GLFW_KEY_W:
                    if (keyEvent.action == GLFW_PRESS)
                        camera.relVelocity.z += -1;
                    else if (keyEvent.action == GLFW_RELEASE)
                        camera.relVelocity.z -= -1;
                    break;
                case GLFW_KEY_S:
                    if (keyEvent.action == GLFW_PRESS)
                        camera.relVelocity.z += 1;
                    else if (keyEvent.action == GLFW_RELEASE)
                        camera.relVelocity.z -= 1;
                    break;
                case GLFW_KEY_A:
                    if (keyEvent.action == GLFW_PRESS)
                        camera.relVelocity.x += -1;
                    else if (keyEvent.action == GLFW_RELEASE)
                        camera.relVelocity.x -= -1;
                    break;
                case GLFW_KEY_D:
                    if (keyEvent.action == GLFW_PRESS)
                        camera.relVelocity.x += 1;
                    else if (keyEvent.action == GLFW_RELEASE)
                        camera.relVelocity.x -= 1;
                    break;
                case GLFW_KEY_LEFT_SHIFT:
                    if (keyEvent.action == GLFW_PRESS)
                        camera.relVelocity.y += -1;
                    else if (keyEvent.action == GLFW_RELEASE)
                        camera.relVelocity.y -= -1;
                    break;
                case GLFW_KEY_SPACE:
                    if (keyEvent.action == GLFW_PRESS) {
                        int currTime = glfwGetTime();
                        int dt = currTime - lastSpacePress;
                        if (dt >= 0 && dt < 1) {
                            camera.isFlying = !camera.isFlying;
                            camera.relVelocity.y = 0;
                        }

                        lastSpacePress = currTime;
                        camera.relVelocity.y += 5;
                    }
                    else if (keyEvent.action == GLFW_RELEASE)
                        if (!camera.isFalling || camera.isFlying)
                            camera.relVelocity.y -= 5;
                        else
                            camera.relVelocity.y = 0;
                    break;
                default:
                    break;
            }

            updateSelectedBlock(camera, transformation);
            m_systemManager->physicsActive = true;
            });
}

void InputSystem::handleMouseButtonEvent(Event* e) {
    MouseButtonEvent mouseButtonEvent = *e->get<MouseButtonEvent>();

    m_systemManager->getRegistry().view<CameraComponent, TransformationComponent>().each(
            [&](CameraComponent& camera, TransformationComponent& transformation) {
            WorldComponent& world = m_systemManager->getCurrentWorld();

            switch (mouseButtonEvent.button) {
                case GLFW_MOUSE_BUTTON_LEFT:
                    if (mouseButtonEvent.action == GLFW_PRESS) {
                        // if block selected
                        if (selectedBlock.valid) {
                            world.setBlock(m_systemManager->getRegistry(), selectedBlock.block, BlockType::BLOCK_AIR);
                        }
                    }
                    break;

                case GLFW_MOUSE_BUTTON_RIGHT:
                    if (mouseButtonEvent.action == GLFW_PRESS) {
                        if (selectedBlock.valid) {
                            glm::ivec3 pos = selectedBlock.block + selectedBlock.face;
                            world.setBlock(m_systemManager->getRegistry(), pos, Block(BlockType::BLOCK_WOOD));
                        }
                    }
                    break;
                default:
                    break;
            }
            });
}

void InputSystem::handleMouseMoveEvent(Event* e) {
    CursorEvent cursorEvent = *e->get<CursorEvent>();

    m_systemManager->getRegistry().view<CameraComponent, VelocityComponent, TransformationComponent>().each(
            [&](auto& camera, auto& velocity, auto& transformation) {
            camera.yaw += cursorEvent.dx * Configuration::getFloatValue("MOUSE_SENSITIVITY");
            camera.pitch -= cursorEvent.dy * Configuration::getFloatValue("MOUSE_SENSITIVITY");

            if (camera.pitch > 89.99) camera.pitch = 89.99;
            else if (camera.pitch < -89.99) camera.pitch = -89.99;

            updateVectors(camera);
            updateSelectedBlock(camera, transformation);
            });
}

void InputSystem::handleScrollEvent(Event* e) {
    ScrollEvent scrollEvent = *e->get<ScrollEvent>();

    m_systemManager->getRegistry().view<CameraComponent>().each(
            [&](auto& camera) {

            camera.fov -= scrollEvent.dy;

            if (camera.fov > 179) camera.fov = 179;
            else if (camera.fov < 1) camera.fov = 1;

            updateProjectionMatrix(camera);
        });
}

void InputSystem::handleFramebufferSizeEvent(Event* e) {
    FramebufferSizeEvent sizeEvent = *e->get<FramebufferSizeEvent>();

    m_systemManager->getRegistry().view<CameraComponent>().each(
            [&](auto& camera) {

            camera.width = sizeEvent.width;
            camera.height = sizeEvent.height;
            updateProjectionMatrix(camera);
        });
}

void InputSystem::updateVectors(CameraComponent& camera) {
    camera.front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    camera.front.y = sin(glm::radians(camera.pitch));
    camera.front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    camera.front = glm::normalize(camera.front);
    camera.right = glm::normalize(glm::cross(camera.front, glm::vec3(0, 1, 0)));
    camera.front_noY = glm::cross(camera.right, glm::vec3(0, 1, 0));
}

void InputSystem::updateViewMatrix(CameraComponent& camera, TransformationComponent& transform) {
    camera.viewMatrix = glm::lookAt(transform.position, transform.position + camera.front, glm::vec3(0, 1, 0));
}

void InputSystem::updateProjectionMatrix(CameraComponent& camera) {
    camera.perspectiveProjection = glm::perspective(glm::radians(camera.fov), camera.width / (float)camera.height, 0.1f, 1000.f);
    camera.orthoProjection = glm::ortho(0.f, camera.width, 0.f, camera.height);
}

void InputSystem::updateSelectedBlock(CameraComponent& camera, TransformationComponent& transformation) {
    WorldComponent& world = m_systemManager->getCurrentWorld();
    glm::vec3 pos = transformation.position;
    Ray r = Ray(pos, camera.front);

    std::vector<glm::ivec3> blocks = r.getAffectedBlocks(5);
    glm::ivec3 block;

    float minLength = FLT_MAX;
    for (auto b : blocks) {
        if (world.getBlock(m_systemManager->getRegistry(), b).type != BlockType::BLOCK_AIR) {
            glm::vec3 diff = pos - (glm::vec3)b;
            float length = glm::length(diff);

            if (length < minLength) {
                block = b;
                minLength = length;
            }
        }
    }

    if (minLength != FLT_MAX) {
        glm::ivec3 intersectionFace = r.getIntersectionFace(block);
        selectedBlock = { block, intersectionFace, true };
    }
    else {
        selectedBlock = { glm::ivec3(), glm::ivec3(), false };
    }
}

void InputSystem::_update(int dt) {
    // TODO make event based
    m_systemManager->getRegistry().view<CameraComponent, TransformationComponent>().each(
            [&](auto& camera, auto& transformation) {
            updateViewMatrix(camera, transformation);
        });
}

InputSystem::InputSystem(SystemManager* systemManager)
    : System(systemManager, 0) {
        ADD_EVENT(handleKeyPressEvent, KEY_EVENT);
        ADD_EVENT(handleMouseMoveEvent, CURSOR_EVENT);
        ADD_EVENT(handleScrollEvent, SCROLL_EVENT);
        ADD_EVENT(handleFramebufferSizeEvent, FRAMEBUFFER_SIZE_EVENT);
        ADD_EVENT(handleMouseButtonEvent, MOUSE_BUTTON_EVENT);	
    }
