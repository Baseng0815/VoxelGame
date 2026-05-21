#include "../../include/Components/ChunkComponent.hpp"

#include "../../include/Configuration.hpp"
#include "../../include/GameData/BlockStates/FluidBlockState.hpp"
#include "../../include/Utility.hpp"

#include <mutex>

void ChunkComponent::setBlock(const Block &block) {
  glm::ivec3 localPos = Utility::getLocalCoords(block.position);
  std::lock_guard<std::shared_mutex> lock{*blockMutex};

  // set state
  switch (block.type) {
  case BlockId::BLOCK_WATER:
    blockStates.setBlockState<FluidBlockState>(block.position);
    break;

  default:
    break;
  }

  blocks[localPos.x][localPos.y][localPos.z] = block.type;
}

Block ChunkComponent::getBlock(int x, int y, int z) const {
  if (Utility::inChunk(x, y, z)) {
    BlockId type = blocks[x][y][z];

    Block block = Block{Utility::getWorldCoords(chunkX, chunkZ, x, y, z), type};
    switch (type) {
    case BlockId::BLOCK_WATER:
      block.state = const_cast<FluidBlockState *>(
          blockStates.getState<FluidBlockState>(x, y, z));
      break;
    default:
      break;
    }
    return block;
  } else {
    return Block{};
  }
}

Block ChunkComponent::getBlock(const glm::ivec3 &position) const {
  return getBlock(position.x, position.y, position.z);
}

Block ChunkComponent::getBlock(int x, int y, int z) {
  if (Utility::inChunk(x, y, z)) {
    BlockId type = blocks[x][y][z];

    Block block =
        Block{Utility::getWorldCoords(chunkX, chunkZ, x, y, z), type, nullptr};
    switch (type) {
    case BlockId::BLOCK_WATER:
      block.state = blockStates.getState<FluidBlockState>(x, y, z);
      break;
    default:
      break;
    }

    return block;
  } else {
    return Block{};
  }
}

Block ChunkComponent::getBlock(const glm::ivec3 &position) {
  return getBlock(position.x, position.y, position.z);
}
