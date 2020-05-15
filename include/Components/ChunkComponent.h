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
public:
    Block*** blocks = nullptr;
    BiomeID** biomes = nullptr;
    std::mutex *blockMutex;

    std::atomic_bool verticesOutdated   = false;
    std::atomic_bool threadActiveOnSelf = false;
    std::atomic_bool chunkBlocksCreated = false;

    int chunkX, chunkZ;

    ChunkComponent(int chunkX, int chunkZ);
    ChunkComponent(const ChunkComponent& other);
    ChunkComponent& operator=(const ChunkComponent& other);

    friend std::ostream& operator<<(std::ostream& stream, const ChunkComponent& chunk);    
};
