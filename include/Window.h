#pragma once

#include "Colors.h"
#include <queue>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Application;
class Event;

class Window {
private:
    GLFWwindow* m_window = nullptr;
    GLFWmonitor* m_primaryMonitor = nullptr;

    // fullscreen toggle
    int m_prevWidth, m_prevHeight;
    int m_prevX, m_prevY;
    bool m_isFullscreen = false;

    std::queue<Event*> m_events;

public:
    Window(Application* app);

    void clear(glm::vec3 clearColor = Colors::Black);
    void display();

    void toggleFullscreen();

    // return true if the key is pressed
    bool getKey(int key) const;
    GLFWwindow* getHandle() const;

    Event* nextEvent();

    void close();
};
