#pragma once

#include <map>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../../include/Resource.h"

// a class used for all shader programs
class Shader : public Resource {
    protected:
        static const unsigned int NUM_SHADERS = 2;

        GLuint m_program;
        GLuint m_shaders[NUM_SHADERS];

        static std::string loadShader(const std::string& fileName);
        static void checkShaderError(GLuint shader, GLuint flag, bool isProgram, std::string errorMessage);
        static GLuint createShader(const std::string& text, GLenum type);

        std::map<std::string, GLint> m_locations;

        GLint getLocation(const std::string& location);

    public:
        Shader(const std::string& path);
        ~Shader();

        void setAttributes(const std::vector<std::string>& attribs);
        void bind() const;

        void upload(const std::string& location, glm::mat4 value);
        void upload(const std::string& location, glm::vec3 value);
        void upload(const std::string& location, float value);
        void upload(const std::string& location, int value);
};
