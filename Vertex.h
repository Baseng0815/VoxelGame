#pragma once

#include "pch.h"

struct Vertex {
public:
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texCoords;

	Vertex(glm::vec3 position = glm::vec3(), glm::vec3 normal = glm::vec3(),
		glm::vec3 color = glm::vec3(), glm::vec2 texCoords = glm::vec2());
};
