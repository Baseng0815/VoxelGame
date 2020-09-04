#pragma once

#include "../Resources/Texture.h"
#include "../Resources/Geometry.h"

struct SkyboxComponent {
    Geometry geometry;
    const Texture *texture;
};
