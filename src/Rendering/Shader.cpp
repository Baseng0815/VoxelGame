#include "../../include/Rendering/Shader.h"

#include <fstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "../include/Configuration.h"

// Private functions

std::string Shader::loadShader(const std::string& fileName) {
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

GLuint Shader::createShader(const std::string& text, GLenum type) {
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

GLint Shader::getLocation(const std::string& location) {
    auto it = m_locations.find(location);
    if (it == m_locations.end()) {
        GLint locationId = glGetUniformLocation(m_program, location.c_str());
        m_locations.insert(std::make_pair(location, locationId));
        return locationId;
    } else
        return it->second;
}

// Public functions
Shader::Shader(const std::string& file) {
    m_program = glCreateProgram();
    m_shaders[0] = createShader(loadShader(Configuration::getStringValue("ResourceBasePath") + file + ".vert"), GL_VERTEX_SHADER);
    m_shaders[1] = createShader(loadShader(Configuration::getStringValue("ResourceBasePath") + file + ".frag"), GL_FRAGMENT_SHADER);

    for (unsigned int i = 0; i < NUM_SHADERS; i++)
        glAttachShader(m_program, m_shaders[i]);

    glLinkProgram(m_program);
    checkShaderError(m_program, GL_LINK_STATUS, GL_TRUE, "Error: Program failed to link: ");
    glValidateProgram(m_program);
    checkShaderError(m_program, GL_VALIDATE_STATUS, GL_TRUE, "Error: Program is invalid: ");
}

Shader::~Shader() {
    for (unsigned int i = 0; i < NUM_SHADERS; i++) {
        glDetachShader(m_program, m_shaders[i]);
        glDeleteShader(m_shaders[i]);
    }
    glDeleteProgram(m_program);
}

void Shader::setAttributes(const std::vector<std::string>& attribs) {
    bind();
    for (int i = 0; i < attribs.size(); i++) {
        glBindAttribLocation(m_program, i, attribs[i].c_str());
    }
}

void Shader::bind() const {
    glUseProgram(m_program);
}

void Shader::upload(const std::string& location, glm::mat4 value) {
    glUniformMatrix4fv(getLocation(location), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::upload(const std::string& location, glm::vec3 value) {
    glUniform3f(getLocation(location), value.x, value.y, value.z);
}

void Shader::upload(const std::string& location, glm::vec4 value) {
    glUniform4f(getLocation(location), value.x, value.y, value.z, value.w);
}

void Shader::upload(const std::string& location, float value) {
    glUniform1f(getLocation(location), value);
}

void Shader::upload(const std::string& location, int value) {
    glUniform1i(getLocation(location), value);
}
