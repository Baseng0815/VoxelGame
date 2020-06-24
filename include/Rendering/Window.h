#pragma once

#include <queue>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct KeyEvent;
struct FramebufferSizeEvent;

class Application;

class Window {
private:
    GLFWwindow* m_window = nullptr;
    GLFWmonitor* m_primaryMonitor = nullptr;

    // fullscreen toggle
    int m_prevWidth, m_prevHeight;
    int m_prevX, m_prevY;
    bool m_isFullscreen = false;

    void handleKeyPress(const KeyEvent&);
    void handleFramebufferSize(const FramebufferSizeEvent&);

public:
    Window(Application* app, int width, int height);

    void clear(glm::vec3 clearColor = glm::vec3(0, 0, 0));
    void display();

    void toggleFullscreen();
    void enableCursor();
    void disableCursor();

    // return true if the key is pressed
    bool getKey(int key) const;
    GLFWwindow* getHandle() const;

    int getWidth() const;
    int getHeight() const;

    void close();
};
