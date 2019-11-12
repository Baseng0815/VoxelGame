#pragma once

#include "pch.h"

class RenderSkybox {
private:
	GLuint m_vao;
	GLuint m_vbo, m_ebo;

public:
	void init();
	void cleanUp();

	void render();
};
