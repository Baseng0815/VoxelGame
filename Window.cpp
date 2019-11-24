#include "pch.h"

#include "Window.h"
#include "Definitions.h"
#include "EventHandler.h"

void Window::init(Application* app) {
	glfwInit();

	m_window = glfwCreateWindow(Definitions::WINDOW_WIDTH, Definitions::WINDOW_HEIGHT, "GLFW window", nullptr, nullptr);
	if (!m_window)
		throw std::runtime_error("Failed to create GLFW window.");

	m_primaryMonitor = glfwGetPrimaryMonitor();

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowUserPointer(m_window, app);

	glfwSetKeyCallback(m_window, EventHandler::keyCallback);
	glfwSetCursorPosCallback(m_window, EventHandler::cursorPosCallback);
	glfwSetScrollCallback(m_window, EventHandler::scrollCallback);
	glfwSetFramebufferSizeCallback(m_window, EventHandler::framebufferSizeCallback);

	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);
}

void Window::clear(Color clearColor) {
	glClearColor(clearColor.r, clearColor.g, clearColor.b, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void Window::display() {
	glfwSwapBuffers(m_window);
}

void Window::toggleFullscreen() {
	if (m_isFullscreen) {
		glfwSetWindowMonitor(m_window, nullptr, m_prevX, m_prevY, m_prevWidth, m_prevHeight, 0);

		glViewport(0, 0, m_prevWidth, m_prevHeight);

		m_isFullscreen = false;
	}
	else {
		glfwGetWindowSize(m_window, &m_prevWidth, &m_prevHeight);
		glfwGetWindowPos(m_window, &m_prevX, &m_prevY);

		const GLFWvidmode* vidMode = glfwGetVideoMode(m_primaryMonitor);
		glfwSetWindowMonitor(m_window,  m_primaryMonitor, 0, 0,
			vidMode->width, vidMode->height, 0);

		glViewport(0, 0, vidMode->width, vidMode->height);

		m_isFullscreen = true;
	}
}

bool Window::getKey(int key) const {
	return glfwGetKey(m_window, key);
}

void Window::close() {
	glfwDestroyWindow(m_window);
}