#include "pch.h"

#include "EventHandler.h"
#include "Application.h"

void EventHandler::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Application* application = (Application*)glfwGetWindowUserPointer(window);

	switch (key) {
		case GLFW_KEY_ESCAPE:
			if (action == GLFW_PRESS)
				application->stop();

		default:
			break;
	}
}