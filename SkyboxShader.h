#pragma once

#include "Shader.h"

class SkyboxShader : public Shader {
private:
	GLint m_locationProjectionMatrix;
	GLint m_locationViewMatrix;

public:
	void init();

	void uploadProjectionMatrix(glm::mat4 projectionMatrix);
	void uploadViewMatrix(glm::mat4 viewMatrix);
};
