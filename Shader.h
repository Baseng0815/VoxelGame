#pragma once

#include "pch.h"

// a class used for all shader programs
class Shader {
private:
	static const unsigned int NUM_SHADERS = 2;

	GLuint m_program = 0;
	GLuint m_shaders[NUM_SHADERS] = { 0 };

	std::map<std::string, int> m_uniformLocations;

	static std::string loadShader(std::string fileName);
	static void checkShaderError(GLuint shader, GLuint flag, bool isProgram, std::string errorMessage);
	static GLuint createShader(std::string text, GLenum type);

public:
	~Shader();

	// fragment and vertex shader need to have same file name
	// extensions used are .vert for vertex and .frag for fragment shaders
	// takes a description of attributes like color or normals
	void init(std::string filename, std::vector<std::string> attribs);

	// finds the uniform and adds its location together
	// with its name to an std::map
	// has to be called after init
	void addUniform(std::string name);
	
	// uploads a value to the shader
	void upload(std::string uniform, glm::mat4 value);
	void upload(std::string uniform, glm::vec3 value);
	void upload(std::string uniform, float value);

	void bind();

};

