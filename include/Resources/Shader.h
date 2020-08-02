#pragma once

#include <map>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Resource.h"

struct Color;
struct PointLight;
struct DirectionalLight;
struct Material;

// a class used for all shader programs
class Shader : public Resource {
    protected:
        GLuint m_program;

        static std::string loadShader(const std::string& fileName);
        static void checkShaderError(GLuint shader, GLuint flag, bool isProgram, std::string errorMessage);
        static GLuint createShader(const std::string& text, GLenum type);

        mutable std::map<std::string, GLint> m_locations;

        GLint getLocation(const std::string& location) const;

        void release() override;

    public:
        Shader(const std::string& vertex, const std::string& fragment);
        Shader(const std::string& file);
        ~Shader();

        Shader(Shader&&) noexcept;
        Shader &operator=(Shader&&) noexcept;

        void setAttributes(const std::vector<std::string>& attribs) const;
        void bind() const;

        void upload(const std::string& location, const glm::mat4& value) const;
        void upload(const std::string& location, const glm::vec3& value) const;
        void upload(const std::string& location, const glm::vec4& value) const;
        void upload(const std::string& location, float value) const;
        void upload(const std::string& location, int value) const;
        void upload(const std::string& location, const Color& color) const;
        void upload(const std::string& location, const PointLight& pointLight) const;
        void upload(const std::string& location, const DirectionalLight& dirLight) const;
        void upload(const std::string& location, const Material& material) const;
};
