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
	m_locationDirLightDir = glGetUniformLocation(m_program, "directionalLight.dir");
	m_locationDirLightColor = glGetUniformLocation(m_program, "directionalLight.color");
	m_locationDirLightAmbientStrength = glGetUniformLocation(m_program, "directionalLight.ambientStrength");
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

void BlockShader::uploadDirectionalLight(DirectionalLight light) {
	upload(m_locationDirLightDir, light.direction);
	upload(m_locationDirLightColor, light.color);
	upload(m_locationDirLightAmbientStrength, light.ambientStrength);
}
