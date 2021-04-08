#pragma once
#include "BlockIds.hpp"
#include "../Typedefs.hpp"

#include <vector>
#include <glm/glm.hpp>

// Cube of 1, 4, 64, 512, 4096 blocks
struct ChunkSection {
    static constexpr int SUBSECTIONS_COUNT = 8;
    static constexpr int directionToIndex(const glm::ivec3& direction);
    static constexpr glm::ivec3 getDirection(int index);

    // section size 0, 1, 2, 3, 4
    byte size = 16;
    glm::ivec3 position = glm::ivec3{};

    BlockId blockType = BlockId::BLOCK_AIR;
    ChunkSection* subsections = nullptr;

    ChunkSection();
    ChunkSection(const glm::ivec3& position, byte size, BlockId blockType);
    ChunkSection(const glm::ivec3& position, byte size, ChunkSection* subsections);
    ChunkSection(const ChunkSection& other);
    ChunkSection(ChunkSection&& other);
    ~ChunkSection();

    void split();
    void cleanUp();
    bool hasSubsections() const;

    bool isAir() const;

    char* getData(size_t* length) const;
    static ChunkSection load(char* data, const glm::ivec3& position);

    ChunkSection& operator=(const ChunkSection& other);
};