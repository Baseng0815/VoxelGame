#include "pch.h"

#include "Application.h"

Application::Application() {
	m_window.init(this);
	EventHandler::attachToWindow(m_window);

	// init glew and load function pointers
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	srand(time(NULL));

	Definitions::loadData();

	m_masterRenderer.init();
	m_camera.init(100);
	m_world.init(WorldType::WORLD_NORMAL);

	m_masterRenderer.addText(&m_fpsText);
	m_masterRenderer.addText(&m_renderTimeText);
	m_masterRenderer.addText(&m_cameraPosText);
	m_masterRenderer.addText(&m_cameraFrontText);
	m_masterRenderer.addText(&m_chunkCountText);

	m_fpsText.position = glm::vec2(0, 580);
	m_fpsText.scale = 0.4;
	m_fpsText.color = Colors::Grey;
	m_renderTimeText.position = glm::vec2(0, 560);
	m_renderTimeText.scale = 0.4;
	m_renderTimeText.color = Colors::Grey;
	m_cameraPosText.position = glm::vec2(0, 540);
	m_cameraPosText.scale = 0.4;
	m_cameraPosText.color = Colors::Grey;
	m_cameraFrontText.position = glm::vec2(0, 520);
	m_cameraFrontText.scale = 0.4;
	m_cameraFrontText.color = Colors::Grey;
	m_chunkCountText.position = glm::vec2(0, 500);
	m_chunkCountText.scale = 0.4;
	m_chunkCountText.color = Colors::Grey;
}

Application::~Application() {

}

void Application::run() {
	while (m_isRunning) {
		// event polling
		glfwPollEvents();

		// fps and render time
		if (m_time > 1000) {
			m_fpsText.text = std::to_string(m_frameCounter / (float)m_time * 1000) + " fps";
			m_renderTimeText.text = std::to_string(m_frameTime / m_frameCounter) + " mcrs render time";

			m_frameTime = 0;
			m_frameCounter = 0;
			m_time = 0;
		}

		// time
		int currentTime = glfwGetTime() * 1000.f;
		m_deltaTime = currentTime - m_prevTime;
		m_prevTime = currentTime;
		m_time += m_deltaTime;

		// updating
		m_camera.handleKeys(m_window, m_deltaTime);

		// updating text each frame hurts the performance
		// @TODO remove or change text debug system (event driven maybe?)
		glm::vec3 cp = m_camera.getPosition();
		m_cameraPosText.text = "[" + std::to_string(cp.x) + " - "
			+ std::to_string(cp.y) + " - " + std::to_string(cp.z) + "]";
		cp = m_camera.getFront();
		m_cameraFrontText.text = "[" + std::to_string(cp.x) + " - "
			+ std::to_string(cp.y) + " - " + std::to_string(cp.z) + "]";

		m_world.update(m_deltaTime, m_camera.getPosition());

		int count = m_world.chunkCount();
		m_chunkCountText.text = std::to_string(count) + " active chunk"
			+ (count > 1 ? "s" : "");

		// drawing
		m_window.clear();

		auto startTime = std::chrono::high_resolution_clock::now();
		m_masterRenderer.render(m_world, m_camera);
		auto endTime = std::chrono::high_resolution_clock::now();
		m_frameTime += std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

		m_window.display();

		m_frameCounter++;
	}
}

void Application::stop() {
	m_isRunning = false;
}