#pragma once

#include "Colors.h"

class Application;

class Window {
private:
	GLFWwindow* m_window = nullptr;
	
public:
	// init GLFW
	void init(Application *app, const int width = 800, const int height = 600);

	void clear(Color clearColor = Colors::Black);
	void display();

	// return true if the key is pressed
	bool getKey(int key) const;

	void close();
};
