#include "pch.h"

#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color,
	float ambientStrength) : direction(direction), color(color),
	ambientStrength(ambientStrength) {}