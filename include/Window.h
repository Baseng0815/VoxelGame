#pragma once

#include <queue>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

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

    void handleKeyPress(Event* e);

public:
    Window(Application* app);

    void clear(glm::vec3 clearColor = glm::vec3(0, 0, 0));
    void display();

    void toggleFullscreen();
    void enableCursor();
    void disableCursor();

    // return true if the key is pressed
    bool getKey(int key) const;
    GLFWwindow* getHandle() const;

    Event* nextEvent();

    void close();
};
