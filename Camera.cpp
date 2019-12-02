#include "pch.h"

#include "Camera.h"
#include "EventHandler.h"

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

void Camera::init(float fov) {
	m_fov = fov;

	EventHandler::addCursorPosCallback(std::bind(&Camera::handleCursorPos, this, nullptr, 0, 0, 0, 0));

	handleFramebufferSize(nullptr, m_width, m_height);
	update(glm::vec3(0, 0, 0));
}

void Camera::resize() {
}

void Camera::handleKeys(Application*, int key, int scancode, int action, int mods) {
	float velocity = dt * MOVE_SPEED;

	// L-CTRL used for speed up
	if (window.getKey(GLFW_KEY_LEFT_CONTROL)) velocity *= 10;

	if (window.getKey(GLFW_KEY_SPACE))
		m_position += velocity * glm::vec3(0, 1, 0);
	if (window.getKey(GLFW_KEY_LEFT_SHIFT))
		m_position += velocity * glm::vec3(0, -1, 0);
	if (window.getKey(GLFW_KEY_D))
		m_position += velocity * m_right;
	if (window.getKey(GLFW_KEY_A))
		m_position += velocity * -m_right;
	if (window.getKey(GLFW_KEY_W))
		m_position += velocity * glm::cross(m_right, glm::vec3(0, -1, 0));
	if (window.getKey(GLFW_KEY_S))
		m_position += velocity * glm::cross(m_right, glm::vec3(0, 1, 0));

	recalculateVectors();
}

void Camera::handleCursorPos(Application*, double, double, double dx, double dy) {
	dx *= MOUSE_SENSITIVITY;
	dy *= MOUSE_SENSITIVITY;

	m_yaw += dx;
	m_pitch -= dy;

	if (m_pitch > 89) m_pitch = 89;
	else if (m_pitch < -89) m_pitch = -89;
}

void Camera::handleScroll(Application*, double, double dy) {
	m_fov -= dy;

	if (m_fov > 179) m_fov = 179;
	else if (m_fov < 1) m_fov = 1;

	handleFramebufferSize(nullptr, m_width, m_height);
}

void Camera::handleFramebufferSize(Application*, int width, int height) {
	m_width = width;
	m_height = height;

	m_projectionPerspective = glm::perspective(glm::radians(m_fov), m_width / (float)height, 0.1f, 100000.f);
	m_projectionOrtho = glm::ortho(0.0f, static_cast<GLfloat>(width),
		0.0f, static_cast<GLfloat>(height));
}

glm::mat4 Camera::getProjectionMatrix(ProjectionType type) const {
	if (type == PROJECTION_PERSPECTIVE)
		return m_projectionPerspective;
	else if (type == PROJECTION_ORTHO)
		return m_projectionOrtho;
}

glm::mat4 Camera::getViewMatrix() const {
	return m_viewMatrix;
}