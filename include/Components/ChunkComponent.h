#pragma once

#include <GL/glew.h>
#include <entt/entt.hpp>

#include <mutex>
#include <future>
#include <string>
#include <memory>
#include <iostream>

#include "../Vertex.h"
#include "../TextureAtlas.h"

class Block;

struct ChunkComponent {
    Block*** blocks = nullptr;
    std::mutex *blockMutex = new std::mutex();

    std::atomic_bool verticesOutdated   = false;
    std::atomic_bool threadActiveOnSelf = false;
    std::atomic_bool structureUpdateRequested = false;

    int chunkX, chunkZ;

    ChunkComponent(int chunkX, int chunkZ);
    ChunkComponent(const ChunkComponent& other);
    ChunkComponent& operator=(const ChunkComponent& other);

    friend std::ostream& operator<<(std::ostream& stream, const ChunkComponent& chunk);
};
