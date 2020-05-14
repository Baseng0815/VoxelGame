#include "../../include/Systems/InputSystem.h"

#include "../../include/Configuration.h"
#include "../../include/Systems/SystemManager.h"
#include "../../include/EventDispatcher.h"
#include "../../include/Collision.h"
#include "../../include/Block.h"

#include "../../include/Components/CameraComponent.h"
#include "../../include/Components/TransformationComponent.h"
#include "../../include/Components/VelocityComponent.h"
#include "../../include/Components/ChunkComponent.h"
#include "../../include/Components/WorldComponent.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


void InputSystem::handleKeyPressEvent(Event* e) {
	KeyEvent keyEvent = *e->get<KeyEvent>();

	m_systemManager->getRegistry().view<CameraComponent, VelocityComponent>().each(
		[&](auto& camera, auto& velocity) {
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
				if (keyEvent.action == GLFW_PRESS)
					camera.relVelocity.y += 5;
				else if (keyEvent.action == GLFW_RELEASE)
					if (!camera.isFalling)
						camera.relVelocity.y -= 5;
					else
						camera.relVelocity.y = 0;
				break;
			default:
				break;
			}

			m_systemManager->physicsActive = true;
		});
}

void InputSystem::handleMouseButtonEvent(Event* e) {
	MouseButtonEvent mouseButtonEvent = *e->get<MouseButtonEvent>();

	m_systemManager->getRegistry().view<CameraComponent, TransformationComponent>().each(
		[&](CameraComponent& camera, TransformationComponent& transformation) {
			switch (mouseButtonEvent.button) {
			case GLFW_MOUSE_BUTTON_LEFT:
				if (mouseButtonEvent.action == GLFW_PRESS) {
					m_systemManager->getRegistry().view<WorldComponent>().each(
						[&](WorldComponent& world) {
							Ray cameraRay = Ray(transformation.position, normalize(camera.front));

							std::vector<glm::ivec3> blocks = cameraRay.getAffectedBlocks(5);
							auto it = blocks.begin();

							if (it != blocks.end()) {
								BlockType block = BlockType::BLOCK_AIR;
								glm::ivec3 blockPos;

								do {									
									block = world.getBlock(m_systemManager->getRegistry(), *it).type;
								} while (block == BlockType::BLOCK_AIR && ++it != blocks.end());

								if (it != blocks.end()) {
									std::cout << "\tx: " << (int)(*it).x << " y: " << (int)(*it).y << " z: " << (int)(*it).z << " type: " << Block(block).toString() << std::endl;

									world.setBlock(m_systemManager->getRegistry(), *it, Block());
								}
							}
						});
				}
				break;
			default:
				break;
			}
		});
}

void InputSystem::handleMouseMoveEvent(Event* e) {
	CursorEvent cursorEvent = *e->get<CursorEvent>();

	m_systemManager->getRegistry().view<CameraComponent, VelocityComponent>().each(
		[&](auto& camera, auto& velocity) {
			camera.yaw += cursorEvent.dx * Configuration::getFloatValue("MOUSE_SENSITIVITY");
			camera.pitch -= cursorEvent.dy * Configuration::getFloatValue("MOUSE_SENSITIVITY");

			if (camera.pitch > 89.99) camera.pitch = 89.99;
			else if (camera.pitch < -89.99) camera.pitch = -89.99;

			updateVectors(camera);
		});
}

void InputSystem::handleScrollEvent(Event* e) {
	ScrollEvent scrollEvent = *e->get<ScrollEvent>();

	m_systemManager->getRegistry().view<CameraComponent>().each(
		[&](auto& camera) {

			camera.fov -= scrollEvent.dy;

			if (camera.fov > 179) camera.fov = 179;
			else if (camera.fov < 1) camera.fov = 1;

			camera.isValid = false;
		});
}

void InputSystem::handleFramebufferSizeEvent(Event* e) {
	FramebufferSizeEvent sizeEvent = *e->get<FramebufferSizeEvent>();

	m_systemManager->getRegistry().view<CameraComponent>().each(
		[&](auto& camera) {

			camera.width = sizeEvent.width;
			camera.height = sizeEvent.height;

			camera.isValid = false;
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

void InputSystem::_update(int dt) {

}

InputSystem::InputSystem(SystemManager* systemManager)
	: System(systemManager, 0) {
	ADD_EVENT(handleKeyPressEvent, KEY_EVENT);
	ADD_EVENT(handleMouseMoveEvent, CURSOR_EVENT);
	ADD_EVENT(handleScrollEvent, SCROLL_EVENT);
	ADD_EVENT(handleFramebufferSizeEvent, FRAMEBUFFER_SIZE_EVENT);
	ADD_EVENT(handleMouseButtonEvent, MOUSE_BUTTON_EVENT);
}
