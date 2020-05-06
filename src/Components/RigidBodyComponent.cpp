#include "../../include/Components/RigidBodyComponent.h"

RigidBodyComponent::RigidBodyComponent(Shape* shape, float density) {
	this->shape = shape;

	mass = shape->volume * density;
}

bool RigidBodyComponent::checkCollision(const glm::mat4& transform, const RigidBodyComponent& other, const glm::mat4& otherTransform) const {
	
}