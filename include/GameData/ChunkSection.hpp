#pragma once

#include "SectionPart.hpp"

#include <vector>
#include <glm/glm.hpp>

struct ChunkSection {
    // index in chunk 0 to 15 from bottom to top
    unsigned char index;

    SectionPart blocks;

    void setBlock(const glm::ivec3& position, BlockId blockType);

    BlockId getBlock(const glm::ivec3& position) const;

    byte* getData(size_t* length) const;
};
