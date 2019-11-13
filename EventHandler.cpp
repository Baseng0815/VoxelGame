#include "pch.h"

#include "EventHandler.h"
#include "Application.h"

bool EventHandler::m_firstMouse = true;
double EventHandler::m_prevX;
double EventHandler::m_prevY;

void EventHandler::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Application* application = (Application*)glfwGetWindowUserPointer(window);

	switch (key) {
		case GLFW_KEY_ESCAPE:
			if (action == GLFW_PRESS)
				application->stop();

		case GLFW_KEY_F:
			if (action == GLFW_PRESS) {
				application->m_window.toggleFullscreen();
				m_firstMouse = true;
			}

		case GLFW_KEY_P:
			if (action == GLFW_PRESS)
				application->m_camera._debugPrint();

		default:
			break;
	}
}

void EventHandler::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	if (m_firstMouse) {
		m_prevX = xpos;
		m_prevY = ypos;
		m_firstMouse = false;
		return;
	}

	double dx = xpos - m_prevX;
	double dy = ypos - m_prevY;

	Application* application = (Application*)glfwGetWindowUserPointer(window);

	application->m_camera.handleMouseMotion(dx, dy);

	m_prevX = xpos;
	m_prevY = ypos;
}

void EventHandler::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	Application* application = (Application*)glfwGetWindowUserPointer(window);

	application->m_camera.handleMouseScroll(yoffset);
}

void EventHandler::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	Application* application = (Application*)glfwGetWindowUserPointer(window);

	application->m_masterRenderer.resize(width, height);
	application->m_camera.resize(width, height);
}
