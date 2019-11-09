#include "pch.h"

#include "BlockShader.h"

void BlockShader::init() {
	std::vector<std::string> attribs = {
		"position",
		"normal",
		"uvCoords"
	};

	Shader::init("Resources/Shaders/chunkShader", attribs);

	m_locationModelMatrix = glGetUniformLocation(m_program, "modelMatrix");
	m_locationProjectionMatrix = glGetUniformLocation(m_program, "projectionMatrix");
	m_locationViewMatrix = glGetUniformLocation(m_program, "viewMatrix");
}

void BlockShader::uploadModelMatrix(glm::mat4 modelMatrix) {
	upload(m_locationModelMatrix, modelMatrix);
}

void BlockShader::uploadProjectionMatrix(glm::mat4 projectionMatrix) {
	upload(m_locationProjectionMatrix, projectionMatrix);
}

void BlockShader::uploadViewMatrix(glm::mat4 viewMatrix) {
	upload(m_locationViewMatrix, viewMatrix);
}
