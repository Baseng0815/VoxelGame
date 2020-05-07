#pragma once
#include "../../include/Shape.h"

struct TransformationComponent;

struct RigidBodyComponent {
	float mass = 0;

	glm::vec3 centerOfMass = glm::vec3();	

	glm::vec3 velocity;

	Shape* shape = nullptr;
	BoxCollision* collision = nullptr;

	RigidBodyComponent(Shape*, float, BoxCollision* collision = nullptr);	
};
