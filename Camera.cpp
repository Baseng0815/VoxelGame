#include "pch.h"

#include "Camera.h"

void Camera::recalculateVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(front);
	m_right = glm::normalize(glm::cross(m_front, glm::vec3(0, 1, 0)));
	m_up = glm::normalize(glm::cross(m_right, m_front));

	m_viewMatrix = glm::lookAt(m_position, m_position + m_front, m_up);	
}

void Camera::init(float fov) {
	m_fov = fov;

	resize(m_width, m_height);
	recalculateVectors();
}

glm::vec3 Camera::getPosition() const {
	return m_position;
}

glm::vec3 Camera::getFront() const {
	return m_front;
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

void Camera::resize(int width, int height) {
	m_width = width;
	m_height = height;

	m_projectionPerspective = glm::perspective(glm::radians(m_fov), width / (float)height, 0.1f, 100000.f);
	m_projectionOrtho = glm::ortho(0.0f, static_cast<GLfloat>(width),
		0.0f, static_cast<GLfloat>(height));
}

void Camera::handleKeys(const Window& window, int dt) {
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

void Camera::handleMouseMotion(double dx, double dy) {
	dx *= MOUSE_SENSITIVITY;
	dy *= MOUSE_SENSITIVITY;

	m_yaw += dx;
	m_pitch -= dy;

	if (m_pitch > 89) m_pitch = 89;
	else if (m_pitch < -89) m_pitch = -89;

	recalculateVectors();
}

void Camera::handleMouseScroll(int dy) {
	m_fov -= dy;

	if (m_fov > 179) m_fov = 179;
	else if (m_fov < 1) m_fov = 1;

	resize(m_width, m_height);
}

void Camera::_debugPrint() {
	std::cout << "[" << m_position.x << " " << m_position.y << " " << m_position.z << "]" << std::endl;
	std::cout << "[" << m_front.x << " " << m_front.y << " " << m_front.z << "]" << std::endl;
	std::cout << "[" << m_right.x << " " << m_right.y << " " << m_right.z << "]" << std::endl;
	std::cout << "--------------------------------------------------------" << std::endl;
}