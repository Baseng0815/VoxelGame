#include "pch.h"

#include "Shader.h"

// Private functions

std::string Shader::loadShader(std::string fileName) {
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open()) {
		while (file.good()) {
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else {
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}

void Shader::checkShaderError(GLuint shader, GLuint flag, bool isProgram, std::string errorMessage) {
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE) {
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}

GLuint Shader::createShader(std::string text, GLenum type) {
	GLuint shader = glCreateShader(type);

	if (shader == 0)
		std::cerr << "Error: Failed to create shader." << std::endl;

	const GLchar *p[1];
	GLint		  l[1];
	l[0] = text.length();
	p[0] = text.c_str();

	glShaderSource(shader, 1, p, l);
	glCompileShader(shader);

	checkShaderError(shader, GL_COMPILE_STATUS, GL_FALSE, "Error: Shader compilation failed: ");

	return shader;
}

// Public functions

Shader::~Shader() {
	for (unsigned int i = 0; i < NUM_SHADERS; i++) {
		glDetachShader(m_program, m_shaders[i]);
		glDeleteShader(m_shaders[i]);
	}
	glDeleteProgram(m_program);
}

void Shader::addUniform(std::string name) {
	GLuint location = glGetUniformLocation(m_program, name.c_str());
	m_uniformLocations.insert(std::pair<std::string, GLuint>(name, location));
}

void Shader::init(std::string fileName, std::vector<std::string> attribs) {
	m_program = glCreateProgram();
	m_shaders[0] = createShader(loadShader(fileName + ".vert"), GL_VERTEX_SHADER);
	m_shaders[1] = createShader(loadShader(fileName + ".frag"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; i++)
		glAttachShader(m_program, m_shaders[i]);

	for (int i = 0; i < attribs.size(); i++) 
		glBindAttribLocation(m_program, i, attribs.at(i).c_str());
	
	glLinkProgram(m_program);
	checkShaderError(m_program, GL_LINK_STATUS, GL_TRUE, "Error: Program failed to link: ");
	glValidateProgram(m_program);
	checkShaderError(m_program, GL_VALIDATE_STATUS, GL_TRUE, "Error: Program is invalid: ");
}

void Shader::upload(std::string uniform, glm::mat4 value) {
	auto it = m_uniformLocations.find(uniform);
	if (it == m_uniformLocations.end())
		std::cerr << "Invalid uniform location: " << uniform << std::endl;

	glUniformMatrix4fv(it->second, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::upload(std::string uniform, glm::vec3 value) {
	auto it = m_uniformLocations.find(uniform);
	if (it == m_uniformLocations.end())
		std::cerr << "Invalid uniform location: " << uniform << std::endl;

	glUniform3f(it->second, value.x, value.y, value.z);
}

void Shader::upload(std::string uniform, float value) {
	auto it = m_uniformLocations.find(uniform);
	if (it == m_uniformLocations.end())
		std::cerr << "Invalid uniform location: " << uniform << std::endl;

	glUniform1f(it->second, value);

}

void Shader::bind() {
	glUseProgram(m_program);
}

