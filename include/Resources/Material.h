#pragma once

#include <glm/glm.hpp>

#include "Resource.h"

class Texture;

class Material : public Resource {
    protected:
        void release() override;

    public:
        // texture properties
        const Texture *diffuseMap;
        const Texture *specularMap;

        // color properties
        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
        float shininess;

        Material(const Texture*, const Texture*, glm::vec4, glm::vec4, glm::vec4, float);

        // TODO kind of unnecessary, maybe handle material differently?
        // material does not fit into RAII very well
        ~Material() = default;

        Material(const Material&) = delete;
        Material &operator=(const Material&) = delete;
        Material(Material&&) noexcept;
        Material &operator=(Material&&) noexcept;
};
