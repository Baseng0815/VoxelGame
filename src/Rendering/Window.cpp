//#include "../../include/Rendering/Window.h"
#include "../../include/Configuration.h"
#include "../../include/Events/EventDispatcher.h"

#include <glm/gtc/matrix_transform.hpp>

#ifdef WIN32
#include <stdexcept>
#endif

// TODO prevent camera from receiving events when ALT is pressed
// also move this into the corresponding layer for finer control
void Window::handleKeyPress(const KeyEvent& e)
{
    /*
    if (e.key == GLFW_KEY_LEFT_ALT) {
        if (e.action == GLFW_PRESS) {
            enableCursor();
        } else if (e.action == GLFW_RELEASE) {
            disableCursor();
        }
    }
    */
}

void Window::handleFramebufferSize(const FramebufferSizeEvent& e)
{
    glViewport(0, 0, e.width, e.height);
    Configuration::setValue("WINDOW_WIDTH", e.width);
    Configuration::setValue("WINDOW_HEIGHT", e.height);
}

Window::Window(Application *app, int width, int height)
{
    Configuration::loadConfiguration("Resources/");
    Configuration::setValue("WINDOW_WIDTH", width);
    Configuration::setValue("WINDOW_HEIGHT", height);

    glfwInit();

    m_window = glfwCreateWindow(width, height, "VoxelGame", nullptr, nullptr);
    if (!m_window)
        throw std::runtime_error("Failed to create GLFW window.");

    m_primaryMonitor = glfwGetPrimaryMonitor();

    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwSetWindowUserPointer(m_window, app);

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(0);

    // init glew and load function pointers
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_keyEventHandle = EventDispatcher::onKeyPress.subscribe([&](const KeyEvent& e) {
        handleKeyPress(e);
    });

    m_framebufferSizeHandle = EventDispatcher::onFramebufferSize.subscribe([&](const FramebufferSizeEvent& e) {
        handleFramebufferSize(e);
    });
}

void Window::clear(glm::vec3 clearColor) {
    glClearColor(clearColor.r, clearColor.g, clearColor.b, 1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void Window::display()
{
    glfwSwapBuffers(m_window);
}

void Window::toggleFullscreen()
{
    if (m_isFullscreen) {
        glfwSetWindowPos(m_window, m_prevX, m_prevY);
        glfwSetWindowSize(m_window, m_prevWidth, m_prevHeight);

        glViewport(0, 0, m_prevWidth, m_prevHeight);

        m_isFullscreen = false;
    }
    else {
        glfwGetWindowSize(m_window, &m_prevWidth, &m_prevHeight);
        glfwGetWindowPos(m_window, &m_prevX, &m_prevY);

        const GLFWvidmode *vidMode = glfwGetVideoMode(m_primaryMonitor);
        glfwSetWindowPos(m_window, 0, 0);
        glfwSetWindowSize(m_window, vidMode->width, vidMode->height);

        glViewport(0, 0, vidMode->width, vidMode->height);

        m_isFullscreen = true;
    }
}

void Window::enableCursor()
{
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::disableCursor()
{
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

bool Window::getKey(int key) const
{ return glfwGetKey(m_window, key); }

GLFWwindow *Window::getHandle() const
{ return m_window; }

void Window::close()
{
    glfwDestroyWindow(m_window);
}
