#pragma once

#include "../Rendering/Material.h"
#include "../Rendering/Geometry.h"

struct MeshRenderComponent {
    Material material;
    Geometry geometry;
};
