#pragma once

#include "Camera.h"

class Chunk;

enum class PlayMode : unsigned char {
	MODE_CREATIVE, MODE_SPECTATOR, MODE_SURVIVAL
};

class Player {
private:
    // camera is responsible for matrices and directions
    // while player handles position, velocity and collision itself
    Camera m_camera;
    glm::vec3 m_position, m_front, m_front_noY, m_right;
    bool m_sprinting = false;

    static const glm::vec3 UP;

    glm::vec3 m_velocity;
    bool m_isFalling = false;

    // also handles gravity
    void handleVelocity(int dt);

    void handleKeys(Event* event);

public:
    Player();

    // dt in milliseconds
    void update(int dt);

    void resolveCollisions(const Chunk* chunk);

    const Camera& getCamera() const;

    const glm::vec3& getPosition() const;
    const glm::vec3& getFront() const;
};
