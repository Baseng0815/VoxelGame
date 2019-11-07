#pragma once

#include "EventHandler.h"
#include "MasterRenderer.h"

class Application {
private:
	Window m_window;
	MasterRenderer m_masterRenderer;
	Camera m_camera;

	friend class EventHandler;

	std::vector<Chunk> m_chunks;

	// delta time
	int m_prevTime = 0, m_deltaTime = 0;
	int counter = 0;

	bool m_isRunning = true;

public:
	Application();
	~Application();

	// enter game loop
	void run();

	// set isRunning to false
	void stop();
};
