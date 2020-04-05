#include "../include/Application.h"
#include "../include/EventDispatcher.h"

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

Application::Application() {
    m_window.init(this);
    EventDispatcher::attachToWindow(m_window);

    EventDispatcher::addCallback(std::bind(&Application::handleKeys, this, std::placeholders::_1), EventType::KEY_EVENT);

    // init glew and load function pointers
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    srand(time(NULL));

    Definitions::loadData();

    // init context data here
    m_context.textureAtlas.init("Resources/Textures/textureAtlas0.png");

    m_systemManager.init(&m_context);
}

Application::~Application() {
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

        // updating and drawing
        auto startTime = std::chrono::high_resolution_clock::now();

        m_window.clear();
        m_systemManager.update(m_deltaTime);
        m_window.display();

        auto endTime = std::chrono::high_resolution_clock::now();
        m_frameTime += std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

        m_frameCounter++;
    }
}

void Application::stop() {
	m_isRunning = false;
}
