#pragma once

#include "BlockIds.hpp"

#include <glm/glm.hpp>

// Cube of 1, 4, 64, 512, 4096 blocks
struct SectionPart {
protected:
    static constexpr int SUBSECTIONS_COUNT = 8;

    static constexpr int directionToIndex(const glm::ivec3& direction);
    static constexpr glm::ivec3 indexToDirection(int index);

public:
    // logarithmic length of one edge 0, 1, 2, 3, 4
    int size = 16;
    glm::ivec3 center = glm::ivec3{};

    BlockId blockType = BlockId::BLOCK_AIR;
    SectionPart* subSections = nullptr;

    SectionPart();
    SectionPart(const glm::ivec3& center, int size, BlockId blockType);
    SectionPart(const SectionPart& other);
    SectionPart(SectionPart&& other);
    ~SectionPart();

    BlockId getBlock(const glm::ivec3& position) const;
    void setBlock(const glm::ivec3& position, BlockId block);

    void split();
    void cleanUp();
    bool hasSubsections() const;

    SectionPart& operator=(const SectionPart& other);
};

struct ChunkSection {
    // index in chunk 0 to 15 from bottom to top
    unsigned char index;

    SectionPart blocks;

    void setBlock(const glm::ivec3& position, BlockId blockType);
    BlockId getBlock(const glm::ivec3& position) const;
};
