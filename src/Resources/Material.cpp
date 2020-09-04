#include "../../include/Resources/Material.h"
#include <algorithm>

void Material::release()
{}

Material::Material(const Texture *diffuseMap, const Texture *specularMap, glm::vec4 ambient,
        glm::vec4 diffuse, glm::vec4 specular, float shininess)
    : diffuseMap(diffuseMap), specularMap(specularMap), ambient(ambient),
    diffuse(diffuse), specular(specular), shininess (shininess)
{}

Material::Material(Material &&other) noexcept
: diffuseMap(other.diffuseMap), specularMap(other.specularMap),
    ambient(other.ambient), diffuse(other.diffuse), specular(other.specular),
    shininess(other.shininess)
{
    other.diffuseMap = nullptr;
    other.specularMap = nullptr;
    other.ambient = glm::vec4 {0.f};
    other.diffuse = glm::vec4 {0.f};
    other.specular = glm::vec4 {0.f};
    other.shininess = 0.f;
}

Material &Material::operator=(Material &&other) noexcept
{
    if (this != &other) {
        std::swap(diffuseMap, other.diffuseMap);
        std::swap(specularMap, other.specularMap);
        std::swap(ambient, other.ambient);
        std::swap(diffuse, other.diffuse);
        std::swap(specular, other.specular);
        std::swap(shininess, other.shininess);
    }

    return *this;
}
