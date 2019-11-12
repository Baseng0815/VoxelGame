#include "pch.h"

#include "RenderSkybox.h"

void RenderSkybox::init() {
	float SIZE = 6000;

	const float vertices[] = {
		-SIZE, SIZE, -SIZE,
		-SIZE, -SIZE, SIZE,
		-SIZE, SIZE, SIZE,
		-SIZE, -SIZE, -SIZE,
		SIZE, SIZE, SIZE,
		SIZE, -SIZE, -SIZE,
		SIZE, SIZE, -SIZE,
		SIZE, -SIZE, SIZE
	};

	const unsigned int indices[] = {
		0, 1, 2,
		0, 3, 1,
		0, 4, 6,
		0, 2, 4,
		4, 5, 6,
		4, 7, 5,
		2, 7, 4,
		2, 1, 7,
		6, 3, 0,
		6, 5, 3,
		1, 5, 7,
		1, 3, 5
	};

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void RenderSkybox::cleanUp() {
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
	glDeleteVertexArrays(1, &m_vao);
}

void RenderSkybox::render() {
	glDisable(GL_CULL_FACE);
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	glEnable(GL_CULL_FACE);
}