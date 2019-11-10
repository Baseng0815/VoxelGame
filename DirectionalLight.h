#pragma once

#include "pch.h"

struct DirectionalLight {
	glm::vec3 direction;
	glm::vec3 color;
	float ambientStrength;

	DirectionalLight(glm::vec3 direction = glm::vec3(0, -1, 0), glm::vec3 color = glm::vec3(1, 1, 1),
		float ambientStrength = 0.2);
};
