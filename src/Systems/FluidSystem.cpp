#include "../../include/Systems/FluidSystem.hpp"

#include "../../include/Components/ChunkComponent.hpp"
#include "../../include/Events/EventDispatcher.hpp"
#include "../../include/GameData/Block.hpp"
#include "../../include/GameData/BlockStates/FluidBlockState.hpp"
#include "../../include/World.hpp"

#include "../../include/Configuration.hpp"

#include <iostream>
#include <vector>

void FluidSystem::handleBlockChanged(const BlockChangedEvent &e) const {
  if (Block::isSolid(e.newBlock)) {
    return;
  }

  const auto &[chunk, position] = Utility::GetChunkAndLocal(e.position);
  ChunkComponent &chunkComponent = World::getChunk(m_registry, chunk);

  switch (e.newBlock) {
  case BlockId::BLOCK_WATER: {
    chunkComponent.fluidUpdates.emplace(position);
  } break;
  case BlockId::BLOCK_AIR: {
    // check if water is above or on side
    static constexpr glm::ivec3 offsets[] = {
        glm::ivec3{0, 1, 0}, glm::ivec3{-1, 0, 0}, glm::ivec3{0, 0, 1},
        glm::ivec3{-1, 0, 0}, glm::ivec3{0, 0, -1}};

    for (const auto &offset : offsets) {
      const glm::ivec3 &neighbourPos = e.position + offset;

      const auto &[chunkPos, localPos] =
          Utility::GetChunkAndLocal(e.position + offset);

      ChunkComponent &neighbourChunk = World::getChunk(m_registry, chunkPos);

      if (neighbourChunk.blocks[localPos.x][localPos.y][localPos.z] ==
          BlockId::BLOCK_WATER) {
        neighbourChunk.fluidUpdates.emplace(localPos);
      }
    }
  } break;
  default:
    break;
  }
}

void FluidSystem::spreadFluid(const glm::ivec3 &position,
                              ChunkComponent &chunk) const {
  int x = position.x, y = position.y, z = position.z;
  if (chunk.blocks[x][y][z] != BlockId::BLOCK_WATER) {
    return;
  }

  // spread downwards
  if (position.y > 0 && !Block::isSolid(chunk.blocks[x][y - 1][z])) {
    const glm::ivec3 lowerPosition = position + glm::ivec3(0, -1, 0);

    chunk.blocks[x][y - 1][z] = BlockId::BLOCK_WATER;
    chunk.blockStates.setBlockState<FluidBlockState>(lowerPosition, 8);
    chunk.fluidUpdates.emplace(lowerPosition);
    chunk.verticesOutdated = true;

    return;
  }

  FluidBlockState *state =
      chunk.blockStates.getState<FluidBlockState>(position);

  if (state->level < 7 || state->level == 8) {
    // spread horizontal
    static constexpr glm::ivec3 neighbourOffsets[4] = {
        glm::ivec3(-1, 0, 0), glm::ivec3(0, 0, 1), glm::ivec3(1, 0, 0),
        glm::ivec3(0, 0, -1)};

    for (const auto &offset : neighbourOffsets) {
      glm::ivec3 neighbourPosition = position + offset;
      bool inChunk = Utility::inChunk(neighbourPosition);

      Block neighbourBlock;
      if (!inChunk) {
        neighbourBlock = World::getBlock(
            m_registry,
            Utility::getWorldCoords(glm::ivec2(chunk.chunkX, chunk.chunkZ),
                                    neighbourPosition));
      } else {
        neighbourBlock = chunk.getBlock(neighbourPosition);
      }

      int level = state->level + 1;
      if (state->level == 8) {
        level = 1;
      }

      switch (neighbourBlock.type) {
      case BlockId::BLOCK_WATER: {
        FluidBlockState *neighbourState =
            reinterpret_cast<FluidBlockState *>(neighbourBlock.state);

        if (neighbourState->level > level) {
          neighbourState->level = level;
          neighbourState->flowTime = 0.0f;

          Utility::chunk_execute(m_registry, chunk, neighbourPosition,
                                 [&](ChunkComponent &neighbourChunk,
                                     const glm::ivec3 &neighbourPosition) {
                                   neighbourChunk.fluidUpdates.emplace(
                                       neighbourPosition);
                                   neighbourChunk.verticesOutdated = true;
                                 });
        }
      } break;
      case BlockId::BLOCK_AIR: {
        Utility::chunk_execute(
            m_registry, chunk, neighbourPosition,
            [&](ChunkComponent &neighbourChunk,
                const glm::ivec3 &neighbourPosition) {
              neighbourChunk.blocks[neighbourPosition.x][neighbourPosition.y]
                                   [neighbourPosition.z] = BlockId::BLOCK_WATER;
              neighbourChunk.blockStates.setBlockState<FluidBlockState>(
                  neighbourPosition, level);
              neighbourChunk.fluidUpdates.emplace(neighbourPosition);

              neighbourChunk.verticesOutdated = true;
            });
      } break;
      default:
        break;
      }
    }
  }
}

void FluidSystem::updateWater(int dt, ChunkComponent &chunk) const {
  if (chunk.fluidUpdates.empty()) {
    return;
  }

  for (auto it = chunk.fluidUpdates.begin(); it != chunk.fluidUpdates.end();) {
    Block block = chunk.getBlock(*it);
    FluidBlockState *fluidState =
        reinterpret_cast<FluidBlockState *>(block.state);
    fluidState->flowTime +=
        dt / static_cast<float>(Configuration::WATER_FLOW_TIME);

    if (fluidState->flowTime >= 1.0f && !chunk.threadActiveOnSelf) {
      fluidState->flowTime = 1.0f;
      spreadFluid(*it, chunk);

      it = chunk.fluidUpdates.erase(it);
    } else {
      it++;
    }
  }
}

void FluidSystem::_update(int dt) {
  m_registry.view<ChunkComponent>().each(
      [&](ChunkComponent &chunk) { updateWater(dt, chunk); });
}

FluidSystem::FluidSystem(Registry_T &registry) : System{registry} {
  m_blockChangedHandle = EventDispatcher::onBlockChange.subscribe(
      [&](const BlockChangedEvent &e) { handleBlockChanged(e); });
}
