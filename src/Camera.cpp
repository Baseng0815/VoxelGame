#include "../include/Camera.h"

#include "../include/EventDispatcher.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std::placeholders;

glm::vec3 Camera::update(glm::vec3 position) {
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
    m_up = glm::normalize(glm::cross(m_right, front));

    m_viewMatrix = glm::lookAt(position, position + front, m_up);
    return front;
}

void Camera::resize() {
    m_projectionPerspective = glm::perspective(glm::radians(m_fov), m_width / (float)m_height, 0.1f, 100000.f);
    m_projectionOrtho = glm::ortho(0.0f, static_cast<GLfloat>(m_width),
            0.0f, static_cast<GLfloat>(m_height));
}

Camera::Camera() {
    resize();
    update(glm::vec3(0, 0, 0));
}

void Camera::handleKey(int key, int action) {
    switch (key) {
        case GLFW_KEY_W:
            if (action == GLFW_PRESS)
                m_velocity.z = -1;
            else if (action == GLFW_RELEASE)
                m_velocity.z = 0;
            break;
        case GLFW_KEY_S:
            if (action == GLFW_PRESS)
                m_velocity.z = 1;
            else if (action == GLFW_RELEASE)
                m_velocity.z = 0;
            break;
        case GLFW_KEY_A:
            if (action == GLFW_PRESS)
                m_velocity.x = -1;
            else if (action == GLFW_RELEASE)
                m_velocity.x = 0;
            break;
        case GLFW_KEY_D:
            if (action == GLFW_PRESS)
                m_velocity.x = 1;
            else if (action == GLFW_RELEASE)
                m_velocity.x = 0;
            break;
        case GLFW_KEY_LEFT_SHIFT:
            if (action == GLFW_PRESS)
                m_velocity.y = -1;
            else if (action == GLFW_RELEASE)
                m_velocity.y = 0;
            break;
        case GLFW_KEY_SPACE:
            if (action == GLFW_PRESS)
                m_velocity.y = 1;
            else if (action == GLFW_RELEASE)
                m_velocity.y = 0;
            break;
        default:
            break;
    }
}

void Camera::handleCursorPos(int dx, int dy) {
    dx *= MOUSE_SENSITIVITY;
    dy *= MOUSE_SENSITIVITY;

    m_yaw += dx;
    m_pitch -= dy;

    if (m_pitch > 89) m_pitch = 89;
    else if (m_pitch < -89) m_pitch = -89;
}

void Camera::handleScroll(double dy) {
    m_fov -= dy;

    if (m_fov > 179) m_fov = 179;
    else if (m_fov < 1) m_fov = 1;

    resize();
}

void Camera::handleFramebufferSize(int width, int height) {
    m_width = width;
    m_height = height;
    resize();
}

void Camera::updatePosition(int dt) {
    m_position += m_right * glm::vec3(m_velocity.x, 0, 0) +
                  m_front * glm::vec3(0, 0, m_velocity.z) +
                  glm::vec3(0, m_velocity.y, 0);
}

glm::mat4 Camera::getProjectionMatrix(ProjectionType type) const {
    if (type == PROJECTION_PERSPECTIVE)
        return m_projectionPerspective;
    else if (type == PROJECTION_ORTHO)
        return m_projectionOrtho;
    else
        return glm::mat4(1);
}

glm::mat4 Camera::getViewMatrix() const {
    return m_viewMatrix;
}

glm::vec3 Camera::getPosition() const {
    return m_position;
}
