#include "../../include/Resources/Material.hpp"
#include <algorithm>

void Material::release()
{}

Material::Material(Material &&other) noexcept
: diffuseMap {other.diffuseMap}, specularMap {other.specularMap},
    color {other.color}, shininess {other.shininess}
{
    other.diffuseMap = nullptr;
    other.specularMap = nullptr;
    other.shininess = 0.f;
    other.color = Color::White;
    other.customShader = nullptr;
}

Material &Material::operator=(Material &&other) noexcept
{
    if (this != &other) {
        std::swap(diffuseMap, other.diffuseMap);
        std::swap(specularMap, other.specularMap);
        std::swap(shininess, other.shininess);
        std::swap(color, other.color);
        std::swap(customShader, other.customShader);
    }

    return *this;
}
