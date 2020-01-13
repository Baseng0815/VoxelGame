#include "../include/Player.h"
#include "../include/Definitions.h"
#include "../include/EventDispatcher.h"

const glm::vec3 Player::UP = glm::vec3(0, 1, 0);

#include <iostream>

void Player::handleVelocity(int dt) {
    glm::vec3 prevPos = m_position;

    m_position += (m_velocity.x * m_right
        + m_velocity.y * UP
        + m_velocity.z * m_front_noY)
        * (dt / 1000.f * Definitions::CAM_SPEED)
        * (m_sprinting ? 2.f : 1.f);

    if (m_isFalling) {
        m_velocity.y -= Definitions::G_CONSTANT;
        if (m_velocity.y < -Definitions::MAX_FALL_SPEED)
            m_velocity.y = -Definitions::MAX_FALL_SPEED;
    }

    // check if player has entered a new chunk
    int oldX = (int)(prevPos.x / Definitions::CHUNK_SIZE);
    int oldY = (int)(prevPos.z / Definitions::CHUNK_SIZE);
    int newX = (int)(m_position.x / Definitions::CHUNK_SIZE);
    int newY = (int)(m_position.z / Definitions::CHUNK_SIZE);

    if (oldX != newX || oldY != newY) {
        EnterChunkEvent e(nullptr, oldX, oldY, newX, newY);
        EventDispatcher::raiseEvent(&e);
    }
}

void Player::handleKeys(Event* event) {
    KeyEvent* kE = event->get<KeyEvent>();
    if (kE->key == GLFW_KEY_LEFT_CONTROL)
        m_sprinting = kE->action;

    int delta = kE->action == GLFW_PRESS ? 1 : (kE->action == GLFW_RELEASE ? -1 : 0);
    if (event) switch (kE->key) {
        case GLFW_KEY_W:
            m_velocity.z += delta;
            break;
        case GLFW_KEY_S:
            m_velocity.z -= delta;
            break;
        case GLFW_KEY_D:
            m_velocity.x += delta;
            break;
        case GLFW_KEY_A:
            m_velocity.x -= delta;
            break;
        case GLFW_KEY_SPACE:
            m_velocity.y += delta;
            break;
        case GLFW_KEY_LEFT_SHIFT:
            m_velocity.y -= delta;
            break;

        default:
            break;
    }
}

Player::Player()
    : m_velocity(0), m_position(0), m_camera(120) {

    ADD_EVENT(Player::handleKeys, KEY_EVENT);
}

void Player::update(int dt) {
    // update camera
    m_front = m_camera.update(m_position);
    m_front_noY = glm::cross(m_right, glm::vec3(0, -1, 0));
    m_right = glm::cross(m_front, glm::vec3(0, 1, 0));

    // apply velocity
    handleVelocity(dt);
}

const Camera& Player::getCamera() const {
    return m_camera;
}

const glm::vec3& Player::getPosition() const {
    return m_position;
}

const glm::vec3& Player::getFront() const {
    return m_front;
}
