#pragma once

#include <GL/glew.h>
#include <entt/entt.hpp>

#include <mutex>
#include <future>
#include <string>
#include <iostream>

#include "../Vertex.h"
#include "../TextureAtlas.h"

class Block;

struct ChunkComponent {
    Block*** blocks = nullptr;
    std::mutex blockMutex;

    bool buffersInitialized = false;
    bool verticesOutdated   = false;
    bool buffersOutdated    = false;
    std::vector<std::future<void>> futures;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    std::atomic_bool threadActiveOnSelf = false;

    int chunkX, chunkZ;

    ChunkComponent(int chunkX, int chunkZ);
    ChunkComponent(ChunkComponent&& other);
    ChunkComponent& operator=(const ChunkComponent& other);

    friend std::ostream& operator<<(std::ostream& stream, const ChunkComponent& chunk);
};
