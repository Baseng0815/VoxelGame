#pragma once

#include "../Rendering/Material.h"
#include "../Resources/Geometry.h"

struct MeshRenderComponent {
    Material material;
    Geometry geometry;
};
