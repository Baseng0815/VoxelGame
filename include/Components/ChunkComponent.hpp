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
#include "../GameData/ChunkSection.hpp"

#include "../GameData/BlockStates/BlockStateContainer.hpp"
#include <vector>

struct Cuboid;
class Geometry;

struct ChunkComponent {
    enum States
    {
        IDLE = 0,
        NEEDS_UPDATE = 1,
        VERTICES_OUTDATED = 2,
        SAVE_REQUESTED = 4,
        DELETION_REQUESTED = 8
    };

    std::shared_mutex* blockMutex;
    int chunkX, chunkZ;

    // raw pointer because ChunkComponent needs to be copyable
    // TODO find out if this works using move semantics only
    Geometry* geometryCulled;
    Geometry* geometryNonCulled;
    Geometry* geometryTransparent;

    // max 16 * 16 * 256 = 65535 elements
    BlockStateContainer blockStates;
    States state = States::IDLE;

    // TODO make this more efficient (maybe use octrees?)
    // four bytes blockdata and four bytes block type
    BlockId*** blocks = nullptr;
    // ChunkSection* sections;

    // TODO maybe use chunk-wise biomes and interpolate
    BiomeId** biomes = nullptr;

    bool threadActiveOnSelf = false;

    void setBlock(const Block& block);

    Block getBlock(int x, int y, int z) const;
    Block getBlock(const glm::ivec3& position) const;

    Block getBlock(int x, int y, int z);
    Block getBlock(const glm::ivec3& position);
};

// state operators
ChunkComponent::States operator~(ChunkComponent::States a);
ChunkComponent::States operator|(ChunkComponent::States a, ChunkComponent::States b);
ChunkComponent::States operator&(ChunkComponent::States a, ChunkComponent::States b);
ChunkComponent::States operator^(ChunkComponent::States a, ChunkComponent::States b);
ChunkComponent::States& operator|=(ChunkComponent::States& a, ChunkComponent::States b);
ChunkComponent::States& operator&=(ChunkComponent::States& a, ChunkComponent::States b);
ChunkComponent::States& operator^=(ChunkComponent::States& a, ChunkComponent::States b);
