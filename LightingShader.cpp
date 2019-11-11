#include "pch.h"

#include "LightingShader.h"

void LightingShader::init() {
	std::vector<std::string> attribs = {
		"position",
		"uvCoords"
	};

	Shader::init("Resources/Shaders/lightingShader", attribs);

	bind();
	m_locationLightPos = glGetUniformLocation(m_program, "lights[0].position");
	m_locationLightColor = glGetUniformLocation(m_program, "lights[0].color");

	upload(glGetUniformLocation(m_program, "gPosition"), 0);
	upload(glGetUniformLocation(m_program, "gNormal"), 1);
	upload(glGetUniformLocation(m_program, "gAlbedo"), 2);
}

void LightingShader::setLight(Light light, int index) {
	upload(m_locationLightColor, light.color);
}