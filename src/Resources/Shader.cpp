#include "../../include/Resources/Shader.hpp"

#include <fstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "../../include/Configuration.hpp"
#include "../../include/Color.hpp"
#include "../../include/Rendering/Light.hpp"
#include "../../include/Resources/Material.hpp"

void Shader::release()
{
    glDeleteProgram(m_program);
}

std::string Shader::loadShader(const std::string &fileName)
{
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

void Shader::checkShaderError(GLuint shader, GLuint flag, bool isProgram, std::string errorMessage)
{
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

GLuint Shader::createShader(const std::string &text, GLenum type)
{
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

GLint Shader::getLocation(const std::string &location) const
{
    auto it = m_locations.find(location);
    if (it == m_locations.end()) {
        GLint locationId = glGetUniformLocation(m_program, location.c_str());
        m_locations.insert(std::make_pair(location, locationId));
        return locationId;
    } else
        return it->second;
}

// Public functions
Shader::Shader(const std::string& vertex, const std::string &fragment) {
    std::cout << "loading vertex shader " << vertex << " and fragment shader " << fragment << std::endl;
    m_program = glCreateProgram();
    GLuint shaderVert = createShader(loadShader(Configuration::getStringValue("ResourceBasePath") + vertex), GL_VERTEX_SHADER);
    GLuint shaderFrag = createShader(loadShader(Configuration::getStringValue("ResourceBasePath") + fragment), GL_FRAGMENT_SHADER);

    glAttachShader(m_program, shaderVert);
    glAttachShader(m_program, shaderFrag);

    glLinkProgram(m_program);
    checkShaderError(m_program, GL_LINK_STATUS, GL_TRUE, "Error: Program failed to link");
    glValidateProgram(m_program);
    checkShaderError(m_program, GL_VALIDATE_STATUS, GL_TRUE, "Error: Program is invalid");

    glDeleteShader(shaderVert);
    glDeleteShader(shaderFrag);
}

Shader::Shader(const std::string &file)
    : Shader(file + ".vert", file + ".frag") {}

Shader::~Shader()
{
    release();
}

Shader::Shader(Shader &&other) noexcept
: m_program(other.m_program), m_locations(std::move(other.m_locations))
{
    other.m_program = 0;
}

Shader &Shader::operator=(Shader &&other) noexcept
{
    if (this != &other) {
        release();
        std::swap(m_program, other.m_program);
        std::swap(m_locations, other.m_locations);
    }

    return *this;
}

void Shader::setAttributes(const std::vector<std::string> &attribs) const {
    bind();
    for (int i = 0; i < attribs.size(); i++) {
        glBindAttribLocation(m_program, i, attribs[i].c_str());
    }
}

void Shader::bind() const {
    glUseProgram(m_program);
}

bool Shader::uniformsSet() const
{ return m_uniformsSet; }

void Shader::setUniformState(bool set) const
{ m_uniformsSet = set; }

void Shader::upload(const std::string &location, const glm::mat4 &value) const
{
    glUniformMatrix4fv(getLocation(location), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::upload(const std::string &location, const glm::vec3 &value) const
{
    glUniform3f(getLocation(location), value.x, value.y, value.z);
}

void Shader::upload(const std::string &location, const glm::vec4 &value) const
{
    glUniform4f(getLocation(location), value.x, value.y, value.z, value.w);
}

void Shader::upload(const std::string &location, float value) const
{
    glUniform1f(getLocation(location), value);
}

void Shader::upload(const std::string &location, int value) const
{
    glUniform1i(getLocation(location), value);
}

void Shader::upload(const std::string &location, const Color &color) const
{
    glUniform4f(getLocation(location), color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
}

void Shader::upload(const std::string &location, const PointLight &pointLight) const
{
    upload(location + ".position", pointLight.position);
    upload(location + ".ambient", pointLight.ambient);
    upload(location + ".diffuse", pointLight.diffuse);
    upload(location + ".specular", pointLight.specular);
    upload(location + ".constant", pointLight.constant);
    upload(location + ".linear", pointLight.linear);
    upload(location + ".quadratic", pointLight.quadratic);
}

void Shader::upload(const std::string &location, const DirectionalLight &dirLight) const
{
    upload(location + ".direction", dirLight.direction);
    upload(location + ".ambient", dirLight.ambient);
    upload(location + ".diffuse", dirLight.diffuse);
    upload(location + ".specular", dirLight.specular);
}

void Shader::upload(const std::string &location, const Material &material) const
{
    upload(location + ".ambient", material.ambient);
    upload(location + ".diffuse", material.diffuse);
    upload(location + ".specular", material.specular);
    upload(location + ".shininess", material.shininess);
}
