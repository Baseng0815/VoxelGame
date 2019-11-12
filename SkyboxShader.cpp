#include "pch.h"

#include "SkyboxShader.h"

void SkyboxShader::init() {
	std::vector<std::string> attribs = {
		"position"
	};

	Shader::init("Resources/Shaders/skyboxShader", attribs);

	m_locationProjectionMatrix = glGetUniformLocation(m_program, "projectionMatrix");
	m_locationViewMatrix = glGetUniformLocation(m_program, "viewMatrix");
}

void SkyboxShader::uploadProjectionMatrix(glm::mat4 projectionMatrix) {
	upload(m_locationProjectionMatrix, projectionMatrix);
}

void SkyboxShader::uploadViewMatrix(glm::mat4 viewMatrix) {
	upload(m_locationViewMatrix, viewMatrix);
}