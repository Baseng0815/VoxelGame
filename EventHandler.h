#pragma once

#include "Window.h"

class EventHandler {
private:
	static bool m_firstMouse;
	static double m_prevX, m_prevY;

public:
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

};
