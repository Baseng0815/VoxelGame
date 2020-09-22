#pragma once

#include "../Resources/Texture.hpp"
#include "../Resources/Geometry.hpp"

// TODO make this a mesh renderer
struct SkyboxComponent {
    Geometry geometry;
    const Texture *texture;
};
