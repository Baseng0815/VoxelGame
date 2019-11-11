#pragma once

#include "pch.h"

#include "Window.h"
#include "Definitions.h"

class Camera {
private:
	float m_yaw, m_pitch, m_fov;
	int m_width = Definitions::WINDOW_WIDTH, m_height = Definitions::WINDOW_HEIGHT;
	
	const float MOUSE_SENSITIVITY = 0.2f;
	const float MOUSE_SCROLL = 2.0f;
	const float MOVE_SPEED = 0.01f;

	glm::vec3 m_position, m_front, m_up, m_right;
	glm::mat4 m_projectionMatrix, m_viewMatrix;

	// recalculate view matrix, front, up and right vectors
	// based on yaw and pitch
	void recalculateVectors();

public:
	void init(float fov);

	glm::mat4 getProjectionMatrix() const;
	glm::mat4 getViewMatrix() const;

	void resize(int width, int height);

	// key handler (not event based)
	// window as argument to retrieve current key states
	// dt always in milliseconds
	void handleKeys(const Window& window, int dt);

	void handleMouseMotion(double dx, double dy);

	void handleMouseScroll(int dy);

	void _debugPrint();
};
