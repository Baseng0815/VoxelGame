#pragma once

#include "Window.h"
#include "SystemManager.h"

class Application {
private:
    Window m_window;
    SystemManager m_systemManager;

    friend class EventDispatcher;

    // delta time
    int m_prevTime = 0, m_deltaTime = 0;

    // fps
    int m_time = 0, m_frameCounter = 0;
    bool m_isRunning = true;

    // frametime, in microseconds, only related to the rendering process
    int m_frameTime = 0;

    void handleKeys(Event* event);

public:
    Application();
    ~Application();

    // enter game loop
    void run();

    // set isRunning to false
    void stop();
};
