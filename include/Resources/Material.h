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
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    Material(const Texture*, const Texture*, glm::vec3, glm::vec3, glm::vec3, float);

    // TODO kind of unnecessary, maybe handle material differently?
    // material does not fit into RAII very well
    ~Material() = default;

    Material(Material&&) noexcept;
    Material &operator=(Material&&) noexcept;
};
