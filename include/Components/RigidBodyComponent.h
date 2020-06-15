#pragma once
#include "../Shape.h"
#include "../Collision.h"

struct TransformationComponent;

struct RigidBodyComponent {
    float mass = 0;

    glm::vec3 centerOfMass = glm::vec3();

    Shape* shape = nullptr;
    BoxCollision* collision = nullptr;

    RigidBodyComponent(Shape*, float, BoxCollision* collision = nullptr);	
};
