#include "pch.h"

#include "Window.h"
#include "EventHandler.h"

void Window::init(Application* app, const int width, const int height) {
	glfwInit();

	m_window = glfwCreateWindow(width, height, "GLFW window", nullptr, nullptr);
	if (!m_window)
		throw std::runtime_error("Failed to create GLFW window.");

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowUserPointer(m_window, app);

	glfwSetKeyCallback(m_window, EventHandler::keyCallback);
	glfwSetCursorPosCallback(m_window, EventHandler::cursorPosCallback);
	glfwSetScrollCallback(m_window, EventHandler::scrollCallback);

	glfwMakeContextCurrent(m_window);
}

void Window::clear(Color clearColor) {
	glClearColor(clearColor.r, clearColor.g, clearColor.b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void Window::display() {
	glfwSwapBuffers(m_window);
}

bool Window::getKey(int key) const {
	return glfwGetKey(m_window, key);
}

void Window::close() {
	glfwDestroyWindow(m_window);
}