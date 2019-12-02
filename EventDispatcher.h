#pragma once

#include "Window.h"
#include "Event.h"

class EventDispatcher {
private:
	static bool m_firstMouse;
	static double m_prevX, m_prevY;

	static CallbackId m_cbCounter;
	static std::map<EventType, std::vector<std::pair<CallbackId, Callback>>> m_callbacks;

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

	static void dispatch(Event *e, EventType callbackType);

public:
	static void attachToWindow(const Window& window);

	static CallbackId addCallback(Callback callback, EventType callbackType);
	static void removeCallback(CallbackId callbackId);
};
