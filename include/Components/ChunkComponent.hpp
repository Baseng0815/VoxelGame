#pragma once

#include <GL/glew.h>
#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include <unordered_map>
#include <shared_mutex>
#include <future>
#include <string>
#include <memory>
#include <iostream>

#include "../GameData/BiomeIds.hpp"

struct Block;
struct Cuboid;

class Geometry;

struct ChunkComponent {
    std::shared_mutex *blockMutex;
    int chunkX, chunkZ;

    // raw pointer because ChunkComponent needs to be copyable
    // TODO find out if it works using move semantics only
    Geometry *geometry;

    // TODO make this more efficient (maybe use octrees?)
    Block*** blocks = nullptr;

    // TODO maybe use chunk-wise biomes and interpolate
    BiomeId** biomes = nullptr;

    bool verticesOutdated   = false;
    bool threadActiveOnSelf = false;    
};
