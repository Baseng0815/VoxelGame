#include "pch.h"

#include "Transform.h"

Transform::Transform(glm::vec3 position, glm::vec3 rotation,
	glm::vec3 scale)
	: position(position), rotation(rotation), scale(scale) {}

glm::mat4 Transform::getTransformMatrix() {
	glm::mat4 rotationMatrix =
		glm::rotate(glm::radians(rotation.z), glm::vec3(0, 0, 1)) *
		glm::rotate(glm::radians(rotation.y), glm::vec3(0, 1, 0)) *
		glm::rotate(glm::radians(rotation.x), glm::vec3(1, 0, 0));

	// order of operation
	// first rotate, then scale, then translate
	return glm::translate(position) * glm::scale(scale) * rotationMatrix;
}