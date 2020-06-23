#pragma once

#include "Event.h"
#include "CallbackList.h"

#include "../Rendering/Window.h"

#include <map>
#include <vector>

class EventDispatcher {
private:
    static bool m_firstMouse;
    static double m_prevX, m_prevY;

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

public:
    static void attachToWindow(const Window& window);

    // TODO somehow do this using templates maybe?
    static CallbackList<KeyEvent> onKeyPress;
    static CallbackList<MouseButtonEvent> onMouseButtonPress;
    static CallbackList<CursorEvent> onCursorMove;
    static CallbackList<ScrollEvent> onMouseScroll;
    static CallbackList<FramebufferSizeEvent> onFramebufferSize;

    static void raiseEvent(const KeyEvent&);
    static void raiseEvent(const MouseButtonEvent&);
    static void raiseEvent(const CursorEvent&);
    static void raiseEvent(const ScrollEvent&);
    static void raiseEvent(const FramebufferSizeEvent&);

    // sets mouse state to firstMouse so the first input event is ignored
    static void resetMouseState();
};
