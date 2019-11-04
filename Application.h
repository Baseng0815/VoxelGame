#pragma once

#include "EventHandler.h"

class Application {
private:
	Window m_window;

	bool m_isRunning = true;

public:
	Application();
	~Application();

	// enter game loop
	void run();

	// set isRunning to false
	void stop();
};
