#pragma once

#include "pch.h"

struct Transform {
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	Transform(glm::vec3 position = glm::vec3(0), glm::vec3 rotation = glm::vec3(0),
		glm::vec3 scale = glm::vec3(0));

	glm::mat4 getTransformMatrix();
};
