#pragma once

#include "MeshRenderComponent.hpp"

// Multi.* components because entities can't have the same component multiple times
struct MultiMeshRenderComponent {
    std::vector<MeshRenderComponent> meshRenderComponents;
};
