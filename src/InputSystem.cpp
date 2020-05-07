#include "../include/InputSystem.h"

#include "../include/Configuration.h"
#include "../include/SystemManager.h"
#include "../include/EventDispatcher.h"

#include "../include/Components/CameraComponent.h"
#include "../include/Components/TransformationComponent.h"
#include "../include/Components/VelocityComponent.h"

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
					camera.relVelocity.y += 1;
				else if (keyEvent.action == GLFW_RELEASE)
					camera.relVelocity.y -= 1;
				break;
			default:
				break;
			}

			updateAbsoluteVelocity(camera, velocity);
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
			updateAbsoluteVelocity(camera, velocity);
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

void InputSystem::updateAbsoluteVelocity(CameraComponent& camera, VelocityComponent& velocity) {
	velocity.velocity = camera.relVelocity.x * camera.right
		+ camera.relVelocity.y * glm::vec3(0, 1, 0)
		+ camera.relVelocity.z * camera.front_noY;
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
	camera.viewMatrix = glm::lookAt(transform.position, transform.position + camera.front,
		glm::vec3(0, 1, 0));
}

void InputSystem::updateProjectionMatrix(CameraComponent& camera) {
	camera.projectionMatrix = glm::perspective(glm::radians(camera.fov), camera.width / (float)camera.height, 0.1f, 1000.f);
}

void InputSystem::_update(int dt) {
	// TODO move velocity and movement into physics system
	m_systemManager->getRegistry().view<CameraComponent, TransformationComponent, VelocityComponent>().each(
		[&](auto& camera, auto& transformation, auto& velocity) {
			int prevChunkX = transformation.position.x / Configuration::CHUNK_SIZE;
			int prevChunkZ = transformation.position.z / Configuration::CHUNK_SIZE;

			transformation.position += velocity.velocity * (float)dt * Configuration::getFloatValue("CAMERA_MOVE_SPEED");

			std::cout << velocity.velocity.x << " " << velocity.velocity.y << " " << velocity.velocity.z << std::endl;
			std::cout << "\t" << transformation.position.x << " " << transformation.position.y << " " << transformation.position.z << std::endl;

			int newChunkX = transformation.position.x / Configuration::CHUNK_SIZE;
			int newChunkZ = transformation.position.z / Configuration::CHUNK_SIZE;

			if (newChunkX != prevChunkX || newChunkZ != prevChunkZ) {
				EnterChunkEvent e;
				e.oldX = prevChunkX;
				e.oldZ = prevChunkZ;
				e.newX = newChunkX;
				e.newZ = newChunkZ;

				EventDispatcher::raiseEvent(&e);
			}

			updateViewMatrix(camera, transformation);
		});
}

InputSystem::InputSystem(SystemManager* systemManager)
	: System(systemManager, 0) {
	ADD_EVENT(handleKeyPressEvent, KEY_EVENT);
	ADD_EVENT(handleMouseMoveEvent, CURSOR_EVENT);
	ADD_EVENT(handleScrollEvent, SCROLL_EVENT);
	ADD_EVENT(handleFramebufferSizeEvent, FRAMEBUFFER_SIZE_EVENT);	
}
