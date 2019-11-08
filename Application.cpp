#include "pch.h"

#include "Application.h"

Application::Application() {
	m_window.init(this);

	// init glew and load function pointers
	glewInit();
	glEnable(GL_DEPTH_TEST);

	srand(time(NULL));

	m_masterRenderer.init();

	// generate some mock chunks
	Chunk chunk;
	chunk.initEmpty();
	chunk.position.z = -10;

	for (int x = 0; x < 16; x++)
		for (int y = 0; y < 16; y++)
			for (int z = 0; z < 16; z++)
				if (x % 3 == 0 && y % 3 == 0 && z % 3 == 0)
					chunk.setBlock(x, y, z, Block((BLOCK_TYPE)(rand() % (NUM_BLOCKS - 1) + 1)));

	for (size_t i = 0; i < 100; i++)
		std::cout << (BLOCK_TYPE)(rand() % (NUM_BLOCKS - 1) + 1) << std::endl;

	chunk.updateMesh();

	m_chunks.push_back(chunk);

	m_camera.init(800, 600, 120);
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
		counter += m_deltaTime;

		if (counter > 500) {
			m_camera._debugPrint();
			counter = 0;
		}

		m_camera.handleKeys(m_window, m_deltaTime);

		// drawing
		m_window.clear();

		m_masterRenderer.draw(m_chunks, m_camera);

		m_window.display();
	}
}

void Application::stop() {
	m_isRunning = false;
}