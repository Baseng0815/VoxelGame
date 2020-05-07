#pragma once
#include "../../include/Shape.h"

struct RigidBodyComponent {
	float mass = 0;

	glm::vec3 centerOfMass = glm::vec3();	

	glm::vec3 velocity;

	Shape* shape = nullptr;
	Shape* collision = nullptr;

	RigidBodyComponent(Shape*, float);

	bool checkCollision(const glm::mat4&, const RigidBodyComponent&, const glm::mat4&) const;
};
