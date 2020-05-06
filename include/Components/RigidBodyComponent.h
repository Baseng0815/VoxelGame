#pragma once
#include "../../include/Shape.h"

struct RigidBodyComponent {
	float mass = 0;
	glm::vec3 centerOfMass = glm::vec3();
	glm::vec3 velocity = glm::vec3();

	Shape* shape = nullptr;

	inline RigidBodyComponent(Shape* shape, float density) {
		this->shape = shape;

		mass = shape->volume * density;
		centerOfMass = shape->center;
	}
};