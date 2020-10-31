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
    std::vector<BlockId> blockData;
    std::vector<BlockState*> blockStates;

    // TODO make this more efficient (maybe use octrees?)
    // four bytes blockdata and four bytes block type
    int*** blocks = nullptr;

    // TODO maybe use chunk-wise biomes and interpolate
    BiomeId** biomes = nullptr;

    bool verticesOutdated = false;
    bool threadActiveOnSelf = false;

    void setBlock(int x, int y, int z, const Block& block);
    const Block getBlock(int x, int y, int z) const;
    Block getBlock(int x, int y, int z);

    template<typename s_type>
    void setBlockState(int x, int y, int z, s_type* state);
    template<typename s_type>
    const s_type* getBlockState(int x, int y, int z) const;
    template<typename s_type>
    s_type* getBlockState(int x, int y, int z);
};
