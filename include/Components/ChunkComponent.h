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

#include "../GameData/BiomeIds.h"

struct Block;
struct Cuboid;

struct ChunkComponent {
    std::shared_mutex *blockMutex;
    int chunkX, chunkZ;

    Block*** blocks = nullptr;

    // TODO fix this memory leak
    BiomeId** biomes = nullptr;

    bool verticesOutdated   = false;
    bool threadActiveOnSelf = false;
};
