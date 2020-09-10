#pragma once

#include <glm/glm.hpp>

#include "Resource.h"

class Texture;
class Shader;

class Material : public Resource {
    protected:
        void release() override;

    public:
        // texture properties
        const Texture *diffuseMap   = nullptr;
        const Texture *specularMap  = nullptr;

        // color properties
        float shininess = 0.f;
        glm::vec4 ambient   = glm::vec4 {0.f};
        glm::vec4 diffuse   = glm::vec4 {0.f};
        glm::vec4 specular  = glm::vec4 {0.f};
        const Shader *customShader = nullptr;
        bool useBlending    = false;
        bool useCulling     = true;

        Material() = default;
        // TODO kind of unnecessary, maybe handle material differently?
        // material does not fit into RAII very well
        ~Material() = default;

        Material(const Material&) = delete;
        Material &operator=(const Material&) = delete;
        Material(Material&&) noexcept;
        Material &operator=(Material&&) noexcept;
};
