#include "pch.h"

#include "Application.h"

Application::Application() {
	m_window.init(this);

	// init glew and load function pointers
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	srand(time(NULL));

	Definitions::loadData();

	m_masterRenderer.init();
	m_camera.init(800, 600, 100);
	m_world.init(WORLD_NORMAL);
}

Application::~Application() {

}

void Application::run() {
	while (m_isRunning) {
		// updating
		glfwPollEvents();

		if (m_time > 1000) {
			std::cout << m_frameCounter / (float)m_time * 1000 << " FPS" <<
				"  -  " << m_frameTime / m_frameCounter << " microseconds render time" << std::endl;

			m_frameTime = 0;
			m_frameCounter = 0;
			m_time = 0;
		}

		int currentTime = glfwGetTime() * 1000.f;
		m_deltaTime = currentTime - m_prevTime;
		m_prevTime = currentTime;
		m_time += m_deltaTime;

		m_camera.handleKeys(m_window, m_deltaTime);

		// drawing
		m_window.clear();

		auto startTime = std::chrono::high_resolution_clock::now();
		m_masterRenderer.draw(m_world, m_camera);
		auto endTime = std::chrono::high_resolution_clock::now();
		m_frameTime += std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

		m_window.display();

		m_frameCounter++;
	}
}

void Application::stop() {
	m_isRunning = false;
}