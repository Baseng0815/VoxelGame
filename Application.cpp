#include "pch.h"

#include "Application.h"

Application::Application() {
	m_window.init(this);

	// init glew and load function pointers
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	srand(time(NULL));

	m_masterRenderer.init();
	m_camera.init(800, 600, 100);
	m_world.init(WORLD_FLAT);
}

Application::~Application() {

}

void Application::run() {
	while (m_isRunning) {
		// updating
		glfwPollEvents();
		int currentTime = glfwGetTime() * 1000.f;
		m_deltaTime = currentTime - m_prevTime;
		m_prevTime = currentTime;
		m_time += m_deltaTime;

		if (m_time > 1000) {
			std::cout << m_frameCounter / (float)m_time * 1000 << " FPS" << std::endl;
			m_frameCounter = 0;
			m_time = 0;
		}

		m_camera.handleKeys(m_window, m_deltaTime);

		// drawing
		m_window.clear();

		m_masterRenderer.draw(m_world, m_camera);

		m_window.display();

		m_frameCounter++;
	}
}

void Application::stop() {
	m_isRunning = false;
}