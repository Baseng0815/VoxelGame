#pragma once

#include "pch.h"

class Skybox {
private:
	GLuint m_texture;

public:
	void init();
	void cleanUp();
	
	void bind();
};
