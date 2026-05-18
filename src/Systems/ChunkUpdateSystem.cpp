#include "../../include/Systems/ChunkUpdateSystem.hpp"

#include "../../include/Components/ChunkComponent.hpp"
#include "../../include/Events/EventDispatcher.hpp"
#include "../../include/GameData/Block.hpp"
#include "../../include/GameData/BlockStates/WaterBlockState.hpp"
#include "../../include/World.hpp"

#include "../../include/Configuration.hpp"

#include <iostream>
#include <vector>

void ChunkUpdateSystem::handleBlockChanged(const BlockChangedEvent &e) {
  // auto [chunkCoords, position] = Utility::GetChunkAndLocal(e.position);
  // ChunkComponent& chunk = World::getChunk(m_registry, chunkCoords);

  for (int dx = -1; dx <= 1; dx++) {
    for (int dy = 0; dy <= 1; dy++) {
      for (int dz = -1; dz <= 1; dz++) {
        if ((dx * dz) == 0) {
          glm::ivec3 dv{dx, dy, dz};

          m_waterUpdates.push_back(
              std::make_pair(Configuration::WATER_FLOW_TIME, e.position + dv));
        }
      }
    }
  }
}

void ChunkUpdateSystem::updateWater(const glm::ivec3 &position) {
  const auto &[chunkPos, blockPos] = Utility::GetChunkAndLocal(position);
  ChunkComponent &chunk = World::getChunk(m_registry, chunkPos);
  Block block = chunk.getBlock(blockPos);
  if (block.type != BlockId::BLOCK_WATER)
    return;

  WaterBlockState *state = reinterpret_cast<WaterBlockState *>(block.state);

  int x = blockPos.x, y = blockPos.y, z = blockPos.z;

  if (y > 1) {
    Block lowerBlock = chunk.getBlock(x, y - 1, z);
    // TODO: Range checking
    if (!lowerBlock.isSolid()) {
      glm::ivec3 lowerPos = glm::ivec3{x, y - 1, z};

      // create new falling state
      chunk.blocks[x][y - 1][z] = BlockId::BLOCK_WATER;
      chunk.blockStates[x][y - 1][z] = new WaterBlockState(8);
      chunk.verticesOutdated = true;

      m_nextWaterUpdates.push_back(
          glm::ivec3{position.x, position.y - 1, position.z});
      return;
    }
  }

  if (state->level < 7 || state->level == 8) {
    int level = state->level == 8 ? 1 : state->level + 1;

    const glm::ivec3 offsets[] = {glm::ivec3{-1, 0, 0}, glm::ivec3{0, 0, -1},
                                  glm::ivec3{1, 0, 0}, glm::ivec3{0, 0, 1}};

    for (const auto &offset : offsets) {
      const glm::ivec3 &neighbourPos = blockPos + offset;

      Utility::chunk_execute(
          m_registry, chunk, neighbourPos.x, neighbourPos.y, neighbourPos.z,
          [&](ChunkComponent &chunk, const int &cx, const int &cy,
              const int &cz) {
            Block tmp = chunk.getBlock(cx, cy, cz);
            if (tmp.type == BlockId::BLOCK_WATER) {
              WaterBlockState *tmpState =
                  reinterpret_cast<WaterBlockState *>(tmp.state);
              if (tmpState->level > level) {
                tmpState->level = level;
                // updateWater(chunk, cx, cy, cz);
                m_nextWaterUpdates.push_back(
                    glm::vec3{chunk.chunkX * Configuration::CHUNK_SIZE + cx, cy,
                              chunk.chunkZ * Configuration::CHUNK_SIZE + cz});
                chunk.verticesOutdated = true;
              }
            }
            // not solid and no water
            else if (!tmp.isSolid()) {
              chunk.blockStates[cx][cy][cz] = new WaterBlockState(level);
              chunk.blocks[cx][cy][cz] = BlockId::BLOCK_WATER;
              // updateWater(chunk, cx, cy, cz);
              m_nextWaterUpdates.push_back(
                  glm::vec3{chunk.chunkX * Configuration::CHUNK_SIZE + cx, cy,
                            chunk.chunkZ * Configuration::CHUNK_SIZE + cz});
              chunk.verticesOutdated = true;
            }
          });
    }
  }
}

void ChunkUpdateSystem::_update(int dt) {
  for (auto it = m_waterUpdates.begin(); it != m_waterUpdates.end();) {
    (*it).first -= dt / 1000.0f;

    if ((*it).first <= 0) {
      glm::ivec3 position = (*it).second;
      updateWater(position);

      it = m_waterUpdates.erase(it);
    } else {
      it++;
    }
  }

  for (auto it = m_nextWaterUpdates.begin(); it != m_nextWaterUpdates.end(); it++) {
    m_waterUpdates.emplace_back(Configuration::WATER_FLOW_TIME, *it);
  }

  m_nextWaterUpdates.clear();
}

ChunkUpdateSystem::ChunkUpdateSystem(Registry_T &registry) : System{registry} {
  m_blockHandle = EventDispatcher::onBlockChange.subscribe(
      [&](const BlockChangedEvent &e) { handleBlockChanged(e); });
}
