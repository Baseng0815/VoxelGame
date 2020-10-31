#pragma once

#include "../Rendering/Light.hpp"
#include <vector>

// contains all lighting data
struct LightingComponent {
    // can be larger than MAX_POINT_LIGHTS; exceeding light sources will be ignored then
    std::vector<PointLight>         pointLights;
    std::vector<DirectionalLight>   dirLights;
};
