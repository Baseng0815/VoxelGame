#pragma once

struct PlayerComponent {
    float maxMovementSpeed = 10.0f;    

    float xAxisInput, yAxisInput, zAxisInput;

    bool isFalling = false;
};
