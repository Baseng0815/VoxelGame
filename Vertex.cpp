#include "pch.h"

#include "Vertex.h"

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 color, glm::vec2 texCoords)
	: position(position), normal(normal), color(color), texCoords(texCoords) {}