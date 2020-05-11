#pragma once

#include "Window.h"
#include "Event.h"

#include <map>
#include <vector>

#define ADD_EVENT(func, type) EventDispatcher::addCallback([=](Event* e){this->func(e);}, type);

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
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    static void dispatch(Event *e);

public:
    static void attachToWindow(const Window& window);

    static CallbackId addCallback(Callback callback, EventType callbackType);
    static void removeCallback(CallbackId callbackId);

    static void raiseEvent(Event* e);
};
