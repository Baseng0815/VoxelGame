#pragma once

#include <GL/glew.h>
#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include <future>
#include <iostream>
#include <memory>
#include <shared_mutex>
#include <string>
#include <unordered_map>

#include "../GameData/BiomeIds.hpp"
#include "../GameData/Block.hpp"

#include "../GameData/BlockStates/BlockStateContainer.hpp"
#include <vector>

struct Cuboid;

class Geometry;

struct ChunkComponent {
    std::shared_mutex* blockMutex;
    int chunkX, chunkZ;

    // raw pointer because ChunkComponent needs to be copyable
    // TODO find out if this works using move semantics only
    Geometry* geometryCulled;
    Geometry* geometryNonCulled;
    Geometry* geometryTransparent;

    // max 16 * 16 * 256 = 65535 elements
    // TODO remove blockVector
    std::vector<BlockId> blockData;
    BlockStateContainer blockStates;
    bool needsUpdate = false;
    
    // TODO make this more efficient (maybe use octrees?)
    // four bytes blockdata and four bytes block type
    short*** blocks = nullptr;

    // TODO maybe use chunk-wise biomes and interpolate
    BiomeId** biomes = nullptr;

    bool verticesOutdated = false;
    bool threadActiveOnSelf = false;

    int getBlockIndex(const BlockId& type) const;

    void setBlock(int x, int y, int z, const Block& block);
    void setBlock(const glm::vec3& position, const Block& block);

    const Block getBlock(int x, int y, int z) const;
    const Block getBlock(const glm::vec3& position) const;

    Block getBlock(int x, int y, int z);
    Block getBlock(const glm::vec3& position);
};
