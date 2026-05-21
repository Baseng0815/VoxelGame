#include "../../../include/GameData/BlockStates/BlockStateContainer.hpp"

#include "../../../include/Components/ChunkComponent.hpp"
#include "../../../include/GameData/BlockStates/FluidBlockState.hpp"

#include <stdexcept>

BlockStateContainer &
BlockStateContainer::operator=(const BlockStateContainer &other) {
  this->blockIndices = other.blockIndices;
  this->stateData = other.stateData;
  return *this;
}

void BlockStateContainer::deleteAll(const ChunkComponent &chunk) {
  for (const auto &[key, index] : blockIndices) {
    const glm::ivec3 &position = getPosition(key);

    switch (chunk.blocks[position.x][position.y][position.z]) {
    case BlockId::BLOCK_WATER:
      delete reinterpret_cast<FluidBlockState *>(stateData[index]);
      break;
    default:
      break;
    }
  }

  blockIndices.clear();
  stateData.clear();
}
