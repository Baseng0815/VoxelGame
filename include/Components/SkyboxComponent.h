#pragma once

#include "../Resources/Texture.h"
#include "../Resources/Geometry.h"

// TODO make this a mesh renderer
struct SkyboxComponent {
    Geometry geometry;
    const Texture *texture;
};
