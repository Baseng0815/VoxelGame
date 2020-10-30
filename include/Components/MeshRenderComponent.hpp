#pragma once

#include "../Resources/Material.hpp"
#include "../Resources/Geometry.hpp"

struct MeshRenderComponent {
    const Material *material;
    const Geometry *geometry;

    int order = 0;
};
