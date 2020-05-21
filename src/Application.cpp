#include "../include/Application.h"

#include "../include/Configuration.h"
#include "../include/EventDispatcher.h"
#include "../include/ResourceManager.h"

#include "../include/Components/CameraComponent.h"

#include <iostream>

void Application::handleKeys(Event* event) {
    KeyEvent* kE = event->get<KeyEvent>();

    switch (kE->key) {
        case GLFW_KEY_ESCAPE:
            if (kE->action == GLFW_PRESS)
                m_isRunning = false;
            break;

        case GLFW_KEY_F:
            if (kE->action == GLFW_PRESS)
                m_window.toggleFullscreen();
            break;

        default:
            break;
    }
}

Application::Application()
    : m_window(this, Configuration::INITIAL_WINDOW_WIDTH, Configuration::INITIAL_WINDOW_HEIGHT),
    m_systemManager() {
    srand(time(NULL));

    EventDispatcher::attachToWindow(m_window);
    ADD_EVENT(handleKeys, KEY_EVENT);

    // raise beginning events
    EnterChunkEvent e;
    e.newX = e.newZ = e.oldX = e.oldZ = 0;
    EventDispatcher::raiseEvent(&e);
}

Application::~Application() {
    ResourceManager::freeResources();
    m_window.close();
    glfwTerminate();
}

void Application::run() {
    while (m_isRunning) {
        // event polling
        glfwPollEvents();

        // fps and render time
        if (m_time > 1000) {
            std::cout << m_frameCounter / (float)m_time * 1000 << " fps" << std::endl;
            m_frameTime = 0;
            m_frameCounter = 0;
            m_time = 0;
        }

        // time
        int currentTime = glfwGetTime() * 1000.f;
        m_deltaTime = currentTime - m_prevTime;
        m_prevTime = currentTime;
        m_time += m_deltaTime;

        auto startTime = std::chrono::high_resolution_clock::now();

        m_window.clear();

        // updating and drawing entities
        m_systemManager.update(m_deltaTime);

        // drawing GUI
        m_systemManager.getRegistry().view<CameraComponent>().each(
            [=](const auto& camera) {
            m_gui.draw(camera);
        });

        m_window.display();

        auto endTime = std::chrono::high_resolution_clock::now();
        m_frameTime += std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

        m_frameCounter++;
    }
}

void Application::stop() {
	m_isRunning = false;
}
