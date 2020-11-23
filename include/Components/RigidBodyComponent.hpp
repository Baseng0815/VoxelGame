#pragma once

struct RigidBodyComponent {
    float mass = 0.0f;

    bool isFalling = false;
    bool gravityApplies = true;
};
