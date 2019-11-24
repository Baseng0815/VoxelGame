#pragma once

#include "World.h"
#include "EventHandler.h"
#include "MasterRenderer.h"

class Application {
private:
	Window m_window;
	MasterRenderer m_masterRenderer;

	Text m_cameraPosText, m_cameraFrontText;
	Camera m_camera;

	Text m_chunkCountText;
	World m_world;

	friend class EventHandler;

	// delta time
	int m_prevTime = 0, m_deltaTime = 0;

	// fps
	Text m_fpsText, m_renderTimeText;
	int m_time = 0, m_frameCounter = 0;
	bool m_isRunning = true;

	// frametime, in microseconds, only related to the rendering process
	int m_frameTime = 0;

public:
	Application();
	~Application();

	// enter game loop
	void run();

	// set isRunning to false
	void stop();
};
