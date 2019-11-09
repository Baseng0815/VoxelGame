#pragma once

#include "Shader.h"

class BlockShader : public Shader {
private:
	GLint m_locationModelMatrix;
	GLint m_locationProjectionMatrix;
	GLint m_locationViewMatrix;

public:
	// load shader and get uniform locations
	void init();

	void uploadModelMatrix(glm::mat4 modelMatrix);
	void uploadProjectionMatrix(glm::mat4 projectionMatrix);
	void uploadViewMatrix(glm::mat4 viewMatrix);
};
