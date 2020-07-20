#include "../../include/Systems/InputSystem.h"

#include "../../include/Block.h"
#include "../../include/Utility.h"
#include "../../include/Collision.h"
#include "../../include/Configuration.h"
#include "../../include/Events/EventDispatcher.h"

#include "../../include/Components/ChunkComponent.h"
#include "../../include/Components/WorldComponent.h"
#include "../../include/Components/CameraComponent.h"
#include "../../include/Components/VelocityComponent.h"
#include "../../include/Components/RigidBodyComponent.h"
#include "../../include/Components/TransformationComponent.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


void InputSystem::handleKeyPressEvent(const KeyEvent& e) {
    m_registry->view<CameraComponent, VelocityComponent, TransformationComponent>().each(
            [&](auto& camera, auto& velocity, auto& transformation) {
            switch (e.key) {
            case GLFW_KEY_W:
            if (e.action == GLFW_PRESS)
            camera.relVelocity.z += -1;
            else if (e.action == GLFW_RELEASE)
            camera.relVelocity.z -= -1;
            break;
            case GLFW_KEY_S:
            if (e.action == GLFW_PRESS)
            camera.relVelocity.z += 1;
            else if (e.action == GLFW_RELEASE)
            camera.relVelocity.z -= 1;
            break;
            case GLFW_KEY_A:
            if (e.action == GLFW_PRESS)
            camera.relVelocity.x += -1;
            else if (e.action == GLFW_RELEASE)
            camera.relVelocity.x -= -1;
            break;
            case GLFW_KEY_D:
            if (e.action == GLFW_PRESS)
                camera.relVelocity.x += 1;
            else if (e.action == GLFW_RELEASE)
                camera.relVelocity.x -= 1;
            break;
            case GLFW_KEY_LEFT_SHIFT:
            if (e.action == GLFW_PRESS)
                camera.relVelocity.y += -1;
            else if (e.action == GLFW_RELEASE)
                camera.relVelocity.y -= -1;
            break;
            case GLFW_KEY_SPACE:
            if (e.action == GLFW_PRESS) {
                int currTime = glfwGetTime();
                int dt = currTime - lastSpacePress;
                if (dt >= 0 && dt < 1) {
                    camera.isFlying = !camera.isFlying;
                    camera.relVelocity.y = 0;
                }

                lastSpacePress = currTime;
                camera.relVelocity.y += 5;
            }
            else if (e.action == GLFW_RELEASE)
                if (!camera.isFalling || camera.isFlying)
                    camera.relVelocity.y -= 5;
                else
                    camera.relVelocity.y = 0;
            break;
            default:
            break;
            }

            updateSelectedBlock(camera, transformation);
            });
}

void InputSystem::handleMouseButtonEvent(const MouseButtonEvent& e) {
    m_registry->view<CameraComponent, TransformationComponent>().each(
            [&](CameraComponent& camera, TransformationComponent& transformation) {
            // TODO reimplement this
            /*
               WorldComponent& world = m_systemManager->getCurrentWorld();

               switch (e.button) {
               case GLFW_MOUSE_BUTTON_LEFT:
               if (e.action == GLFW_PRESS) {
            // if block selected
            if (selectedBlock.valid) {
            world.setBlock(*m_registry, selectedBlock.block, BlockType::BLOCK_AIR);
            }
            }
            break;

            case GLFW_MOUSE_BUTTON_RIGHT:
            if (e.action == GLFW_PRESS) {
            if (selectedBlock.valid) {
            glm::vec3 pos = selectedBlock.block + selectedBlock.face;
            world.setBlock(*m_registry, pos, Block(BlockType::BLOCK_WOOD));
            }
            }
            break;
            default:
            break;
            }
            */
            });
}

void InputSystem::handleMouseMoveEvent(const CursorEvent& e) {
    m_registry->view<CameraComponent, VelocityComponent, TransformationComponent>().each(
            [&](auto& camera, auto& velocity, auto& transformation) {
            camera.yaw += e.dx * Configuration::getFloatValue("MOUSE_SENSITIVITY");
            camera.pitch -= e.dy * Configuration::getFloatValue("MOUSE_SENSITIVITY");

            if (camera.pitch > 89.99) camera.pitch = 89.99;
            else if (camera.pitch < -89.99) camera.pitch = -89.99;

            updateVectors(camera);
            updateSelectedBlock(camera, transformation);
            });
}

void InputSystem::handleScrollEvent(const ScrollEvent& e) {
    m_registry->view<CameraComponent>().each(
            [&](auto& camera) {

            camera.fov -= e.dy;

            if (camera.fov > 179) camera.fov = 179;
            else if (camera.fov < 1) camera.fov = 1;

            updateProjectionMatrix(camera);
            });
}

void InputSystem::handleFramebufferSizeEvent(const FramebufferSizeEvent& e) {
    m_registry->view<CameraComponent>().each(
        [&](auto& camera) {

        camera.width = e.width;
        camera.height = e.height;
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
    camera.viewMatrix = glm::lookAt(transform.getPosition(), transform.getPosition() + camera.front, glm::vec3(0, 1, 0));
}

void InputSystem::updateProjectionMatrix(CameraComponent& camera) {
    camera.perspectiveProjection = glm::perspective(glm::radians(camera.fov), camera.width / (float)camera.height, 0.1f, 1000.f);
}

void InputSystem::updateSelectedBlock(CameraComponent& camera, TransformationComponent& transformation) {
    // TODO reimplement this
    /*
       WorldComponent& world = m_systemManager->getCurrentWorld();
       glm::vec3 pos = transformation.position;

       Ray r = Ray(pos, camera.front);

       std::vector<glm::vec3> blocks = r.getIntersectionBlocks(5);
       glm::vec3 block;

       float minLength = FLT_MAX;
       for (auto b : blocks) {
       try{
       if (world.getBlock(*m_registry, b).type != BlockType::BLOCK_AIR) {
       glm::vec3 diff = pos - (glm::vec3)b;
       float length = glm::length(diff);

       if (length < minLength) {
       block = b;
       minLength = length;
       }
       }
       }
       catch(std::out_of_range) {
       continue;
       }
       }

       if (minLength != FLT_MAX) {
       glm::vec3 intersectionFace = r.getIntersectionFace(block);
       selectedBlock = { block, intersectionFace, true };
       }
       else {
       selectedBlock = { glm::vec3(), glm::vec3(), false };
       }
       */
}

void InputSystem::_update(int dt) {
    // TODO make event based
    m_registry->view<CameraComponent, TransformationComponent>().each(
            [&](auto& camera, auto& transformation) {
            updateViewMatrix(camera, transformation);
            });
}

InputSystem::InputSystem(entt::registry* registry)
    : System(registry, 0) {
        // create camera and update projection matrix
        // TODO put into extra system
        entt::entity entity = m_registry->create();
        m_registry->emplace<CameraComponent>(entity, 90.f, Configuration::getFloatValue("WINDOW_WIDTH"), Configuration::getFloatValue("WINDOW_HEIGHT"));
        m_registry->emplace<TransformationComponent>(entity, glm::vec3(0, 100, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
        m_registry->emplace<VelocityComponent>(entity);
        BoxCollision* cameraCollision = new BoxCollision(glm::vec3(-0.5f, -1.5f, -0.5f), 1, 2, 1);
        m_registry->emplace<RigidBodyComponent>(entity, new Shape(std::vector<Triangle>()), 0, cameraCollision);

        m_registry->view<CameraComponent>().each(
            [&](auto& camera) {
            updateProjectionMatrix(camera);
        });

        EventDispatcher::onKeyPress += [this](const KeyEvent &e) {
            handleKeyPressEvent(e);
        };
        EventDispatcher::onCursorMove += [this](const CursorEvent &e) {
            handleMouseMoveEvent(e);
        };
        EventDispatcher::onMouseScroll += [this](const ScrollEvent &e) {
            handleScrollEvent(e);
        };
        EventDispatcher::onFramebufferSize += [this](const FramebufferSizeEvent &e) {
            handleFramebufferSizeEvent(e);
        };
        EventDispatcher::onMouseButtonPress += [this](const MouseButtonEvent&e) {
            handleMouseButtonEvent(e);
        };
    }
