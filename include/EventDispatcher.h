#pragma once

#include "Event.h"
#include "Rendering/Window.h"

#include <map>
#include <vector>

#define CB_FUN(func) ([=](Event* e){this->func(e);})

class EventDispatcher {
private:
    static bool m_firstMouse;
    static double m_prevX, m_prevY;

    static CallbackId m_cbCounter;
    static std::map<EventType, std::vector<Callback>> m_callbacks;

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    static void dispatch(Event *e);

public:
    static void attachToWindow(const Window& window);

    static CallbackId addCallback(CallbackFun fun, EventType type);
    static void removeCallback(CallbackId id);
    static void setCallbackActive(CallbackId callbackId, bool active);

    // sets mouse state to firstMouse so the first input event is ignored
    static void resetMouseState();
    static void raiseEvent(Event* e);
};
