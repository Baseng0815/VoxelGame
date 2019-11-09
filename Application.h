#pragma once

#include "World.h"
#include "EventHandler.h"
#include "MasterRenderer.h"

class Application {
private:
	Window m_window;
	MasterRenderer m_masterRenderer;
	Camera m_camera;
	World m_world;

	friend class EventHandler;

	// delta time
	int m_prevTime = 0, m_deltaTime = 0;

	// fps
	int m_time = 0, m_frameCounter = 0;

	bool m_isRunning = true;

public:
	Application();
	~Application();

	// enter game loop
	void run();

	// set isRunning to false
	void stop();
};
