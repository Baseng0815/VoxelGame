#include "../include/GUIShader.h"

void GUIShader::init() {
	std::vector<std::string> attribs = {
		"vertex"
	};

	Shader::init("Resources/Shaders/GUIShader", attribs);

	bind();
	m_locationTextColor = glGetUniformLocation(m_program, "textColor");
}

void GUIShader::setTextColor(glm::vec3 color) const {
	upload(m_locationTextColor, color);
}
