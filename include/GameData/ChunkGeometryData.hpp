#pragma once
#include "../Rendering/Vertex.hpp"
#include <vector>
#include <entt/entt.hpp>

struct ChunkGeometryData {
    entt::entity entity;
    std::vector<Vertex> verticesCulled;
    std::vector<Vertex> verticesNonCulled;
    std::vector<Vertex> verticesTransparent;
    std::vector<unsigned int> indicesCulled;
    std::vector<unsigned int> indicesNonCulled;
    std::vector<unsigned int> indicesTransparent;
};