#include "../../include/Resources/Material.h"

#include <iostream>

Material::Material(Texture* diffuseMap, Texture* specularMap, glm::vec3 ambient,
        glm::vec3 diffuse, glm::vec3 specular, float shininess)
    : diffuseMap(diffuseMap), specularMap(specularMap), ambient(ambient),
    diffuse(diffuse), specular(specular), shininess (shininess) {
        std::cout << "creating material" << std::endl;
}


