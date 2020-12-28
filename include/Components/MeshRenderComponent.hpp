#pragma once

#include "../Resources/Material.hpp"
#include "../Resources/Geometry.hpp"

struct MeshRenderComponent {
    const Material *material;
    const Geometry *geometry;

    bool isActive = true;
};
