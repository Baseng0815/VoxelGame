#pragma once

#include <GL/glew.h>
#include <entt/entt.hpp>

#include <mutex>
#include <future>

#include "../Vertex.h"
#include "../TextureAtlas.h"

class Block;

struct ChunkComponent {
    Block*** blocks = nullptr;
    std::mutex blockMutex, vaoMutex;

    bool buffersInitialized = false;
    bool verticesOutdated   = false;
    bool buffersOutdated    = false;
    std::vector<std::future<void>> futures;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    std::atomic_bool threadActiveOnSelf;

    ChunkComponent() = default;
    ChunkComponent(ChunkComponent&& other);
    ChunkComponent& operator=(const ChunkComponent& other);
};
