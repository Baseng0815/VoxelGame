#pragma once

#include "../Resources/Material.h"
#include "../Resources/Geometry.h"

struct MeshRenderComponent {
    const Material *material;
    Geometry geometry;
};
