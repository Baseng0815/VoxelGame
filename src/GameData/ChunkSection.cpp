#include "../../include/GameData/ChunkSection.hpp"

void ChunkSection::setBlock(const glm::ivec3& position, BlockId blockType) {
    blocks.setBlock(position, blockType);
}

BlockId ChunkSection::getBlock(const glm::ivec3& position) const {
    return blocks.getBlock(position);
}

byte* ChunkSection::getData(size_t* length) const {
    return blocks.getData(length);
}