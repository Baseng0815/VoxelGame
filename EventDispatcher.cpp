#include "pch.h"

#include "EventDispatcher.h"
#include "Application.h"

bool EventDispatcher::m_firstMouse = true;
double EventDispatcher::m_prevX;
double EventDispatcher::m_prevY;
CallbackId EventDispatcher::m_cbCounter = 0;

void EventDispatcher::attachToWindow(const Window& window) {
	GLFWwindow* hwnd = window.getHandle();

	glfwSetKeyCallback(hwnd, EventDispatcher::keyCallback);
	glfwSetCursorPosCallback(hwnd, EventDispatcher::cursorPosCallback);
	glfwSetScrollCallback(hwnd, EventDispatcher::scrollCallback);
	glfwSetFramebufferSizeCallback(hwnd, EventDispatcher::framebufferSizeCallback);
}

void EventDispatcher::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Application* application = (Application*)glfwGetWindowUserPointer(window);

	KeyEvent e(application, key, scancode, action, mods);
	dispatch(&e, KeyEvent::TYPE);
}

void EventDispatcher::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	if (m_firstMouse) {
		m_prevX = xpos;
		m_prevY = ypos;
		m_firstMouse = false;
		return;
	}

	double dx = xpos - m_prevX;
	double dy = ypos - m_prevY;

	Application* application = (Application*)glfwGetWindowUserPointer(window);

	CursorEvent e(application, xpos, ypos, dx, dy);
	dispatch(&e, CursorEvent::TYPE);

	m_prevX = xpos;
	m_prevY = ypos;
}

void EventDispatcher::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	Application* application = (Application*)glfwGetWindowUserPointer(window);

	ScrollEvent e(application, xoffset, yoffset);
	dispatch(&e, ScrollEvent::TYPE);
}

void EventDispatcher::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	Application* application = (Application*)glfwGetWindowUserPointer(window);

	FramebufferSizeEvent e(application, width, height);
	dispatch(&e, FramebufferSizeEvent::TYPE);
}

void EventDispatcher::dispatch(Event *e, EventType callbackType) {
	auto cbIt = m_callbacks.find(callbackType);
	if (cbIt == m_callbacks.end()) return;

	for (auto& callback : (*cbIt).second)
		callback.second(e);
}

CallbackId EventDispatcher::addCallback(Callback callback, EventType callbackType) {
	auto& list = m_callbacks.at(callbackType);
	list.push_back(std::make_pair(m_cbCounter, callback));
	m_cbCounter++;
	return m_cbCounter;
}

void EventDispatcher::removeCallback(CallbackId callbackId) {
	for (auto it = m_callbacks.begin(); it != m_callbacks.end(); it++)
		for (auto jt = it->second.begin(); jt != it->second.end(); jt++)
			if (jt->first == callbackId)
				it->second.erase(jt);
}
