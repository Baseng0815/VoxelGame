#pragma once

#include "pch.h"

// used by the master renderer to render the final texture onto
class RenderQuad {
private:
	GLuint m_vao;
	GLuint m_vbo;

public:
	void init();
	void cleanUp();

	void render();
};
