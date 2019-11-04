#pragma once

#include "pch.h"

#include "Vertex.h"

class Mesh {
private:
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ebo;

public:
	Mesh();
	Mesh(std::string path);
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
};
