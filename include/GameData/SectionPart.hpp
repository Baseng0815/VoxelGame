#pragma once
#include "BlockIds.hpp"
#include "../Typedefs.hpp"

#include <vector>
#include <glm/glm.hpp>

// Cube of 1, 4, 64, 512, 4096 blocks
struct SectionPart {
protected:
    static constexpr int SUBSECTIONS_COUNT = 8;

    static constexpr int directionToIndex(const glm::ivec3& direction);
    static constexpr glm::ivec3 indexToDirection(int index);

public:
    // section size 0, 1, 2, 3, 4
    byte size = 16;
    glm::ivec3 center = glm::ivec3{};

    BlockId blockType = BlockId::BLOCK_AIR;
    SectionPart* subSections = nullptr;

    SectionPart();
    SectionPart(const glm::ivec3& center, byte size, BlockId blockType);
    SectionPart(const SectionPart& other);
    SectionPart(SectionPart&& other);
    ~SectionPart();

    BlockId getBlock(const glm::ivec3& position) const;
    void setBlock(const glm::ivec3& position, BlockId block);

    void split();
    void cleanUp();
    bool hasSubsections() const;

    byte* getData(size_t* length) const;

    SectionPart& operator=(const SectionPart& other);
};