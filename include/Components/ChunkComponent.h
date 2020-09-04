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

struct Block;
enum BiomeID : char;

struct ChunkComponent {
    std::shared_mutex *blockMutex;
    int chunkX, chunkZ;

    Block*** blocks = nullptr;
    std::vector<glm::vec3> blockCollisions;
    // TODO fix this memory leak
    BiomeID** biomes = nullptr;

    bool verticesOutdated   = false;
    bool threadActiveOnSelf = false;
};
