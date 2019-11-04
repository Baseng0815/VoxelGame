#include "pch.h"

#include "Application.h"

Application::Application() {
	m_window.init(this);

}

Application::~Application() {

}

void Application::run() {
	while (m_isRunning) {
		// updating
		glfwPollEvents();


		// drawing
		m_window.clear();

		m_window.display();
	}
}

void Application::stop() {
	m_isRunning = false;
}