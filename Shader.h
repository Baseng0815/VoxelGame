#pragma once

#include "pch.h"

// a class used for all shader programs
class Shader {
protected:
	static const unsigned int NUM_SHADERS = 2;

	GLuint m_program = 0;
	GLuint m_shaders[NUM_SHADERS] = { 0 };

	static std::string loadShader(std::string fileName);
	static void checkShaderError(GLuint shader, GLuint flag, bool isProgram, std::string errorMessage);
	static GLuint createShader(std::string text, GLenum type);

	// uploads a value to the shader
	void upload(GLint location, glm::mat4 value);
	void upload(GLint location, glm::vec3 value);
	void upload(GLint location, float value);

	// fragment and vertex shader need to have same file name
	// extensions used are .vert for vertex and .frag for fragment shaders
	// takes a description of attributes like color or normals
	void init(std::string filename, std::vector<std::string> attribs);

public:
	~Shader();

	void bind();
};

