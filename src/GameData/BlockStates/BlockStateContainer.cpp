#include "../../../include/GameData/BlockStates/BlockStateContainer.hpp"

#include "../../../include/Components/ChunkComponent.hpp"
#include "../../../include/GameData/BlockStates/WaterBlockState.hpp"

#include <stdexcept>

BlockStateContainer &
BlockStateContainer::operator=(const BlockStateContainer &other) {
  this->blockIndices = other.blockIndices;
  this->stateData = other.stateData;
  return *this;
}

// void BlockStateContainer::deleteBlockState(const glm::vec3 &position) {
//   try {
//     int key = getKey(position);
//     int index = blockIndices.at(key);
//     delete stateData[index];

//     stateData.erase(stateData.begin() + index);
//     blockIndices.erase(key);

//     for (auto& [_, i] : blockIndices) {
//         if (i > index) {
//             i--;
//         }
//     }
//   } catch (std::out_of_range e) {
//   }
// }

void BlockStateContainer::deleteAll(const ChunkComponent &chunk) {
  for (const auto &[key, index] : blockIndices) {
    const glm::ivec3 &position = getPosition(key);

    switch (chunk.blocks[position.x][position.y][position.z]) {
    case BlockId::BLOCK_WATER:
      delete reinterpret_cast<WaterBlockState *>(stateData[index]);
      break;
    default:
      break;
    }
  }

  blockIndices.clear();
  stateData.clear();
}
