#pragma once

#include "../Resources/Material.h"
#include "../Resources/Geometry.h"

struct MeshRenderComponent {
    Material* material;
    Geometry geometry;
};
