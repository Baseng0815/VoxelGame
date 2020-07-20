#pragma once

#include <glm/glm.hpp>

#include "Resource.h"

class Texture;

struct Material : public Resource {
    // texture properties
    Texture* diffuseMap;
    Texture* specularMap;

    // color properties
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    Material(Texture*, Texture*, glm::vec3, glm::vec3, glm::vec3, float);
};
