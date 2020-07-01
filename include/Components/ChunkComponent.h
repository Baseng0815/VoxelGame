#pragma once

#include <GL/glew.h>
#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include <unordered_map>
#include <mutex>
#include <future>
#include <string>
#include <memory>
#include <iostream>

struct Block;
enum BiomeID : char;

struct ChunkComponent {
    std::mutex *blockMutex;
    int chunkX, chunkZ;

    Block*** blocks = nullptr;
    BiomeID** biomes = nullptr;

    std::atomic_bool verticesOutdated   = false;
    std::atomic_bool threadActiveOnSelf = false;
    std::atomic_bool chunkBlocksCreated = false;

    // TODO make this use aggregate initialization
    ChunkComponent(std::mutex* blockMutex, int chunkX, int chunkZ);
    ChunkComponent(const ChunkComponent& other);
    ChunkComponent& operator=(const ChunkComponent& other);

    friend std::ostream& operator<<(std::ostream& stream, const ChunkComponent& chunk);    
};
