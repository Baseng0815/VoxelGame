#pragma once

#include "Colors.h"

class Application;

class Window {
private:
	GLFWwindow* m_window = nullptr;

	// fullscreen toggle
	int m_prevWidth, m_prevHeight;
	int m_prevX, m_prevY;
	bool m_isFullscreen = false;
	
public:
	// init GLFW
	void init(Application *app, const int width = 800, const int height = 600);

	void clear(Color clearColor = Colors::Black);
	void display();

	void toggleFullscreen();

	// return true if the key is pressed
	bool getKey(int key) const;

	void close();
};
