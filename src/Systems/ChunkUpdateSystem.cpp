#include "../../include/Systems/ChunkUpdateSystem.hpp"

#include "../../include/Components/ChunkComponent.hpp"
#include "../../include/Events/EventDispatcher.hpp"
#include "../../include/GameData/Block.hpp"
#include "../../include/GameData/BlockStates/WaterBlockState.hpp"
#include "../../include/World.hpp"

#include "../../include/Configuration.hpp"

#include <iostream>
#include <vector>

void ChunkUpdateSystem::handleBlockChanged(const BlockChangedEvent& e) {
    // auto [chunkCoords, position] = Utility::GetChunkAndLocal(e.position);
    // ChunkComponent& chunk = World::getChunk(m_registry, chunkCoords);

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = 0; dy <= 1; dy++) {
            for (int dz = -1; dz <= 1; dz++) {
                if ((dx * dz) == 0) {
                    glm::ivec3 dv{dx, dy, dz};

                    m_waterUpdates.push_back(std::make_pair(Configuration::WATER_FLOW_TIME, e.position + dv));
                    // Utility::chunk_execute(m_registry, chunk, cx, cy, cz,
                    //                        [&](ChunkComponent& otherChunk, const int& x, const int& y, const int& z) {
                    //                            BlockId block = otherChunk.blocks[x][y][z];
                    //                            if (block == BlockId::BLOCK_WATER) {
                    //                                updateWater(otherChunk, x, y, z);
                    //                            }
                    //                        });
                }
            }
        }
    }

    // chunk.verticesOutdated = true;
}

void ChunkUpdateSystem::updateWater(const glm::ivec3& position) {
    auto [chunkPos, blockPos] = Utility::GetChunkAndLocal(position);
    ChunkComponent chunk = World::getChunk(m_registry, chunkPos);
    Block block = chunk.getBlock(blockPos);
    if(block.type != BlockId::BLOCK_WATER)
        return;

    WaterBlockState* state = reinterpret_cast<WaterBlockState*>(block.state);

    int x = blockPos.x, y = blockPos.y, z = blockPos.z;

    if (y > 1) {
        Block lowerBlock = chunk.getBlock(x, y - 1, z);
        // TODO: Range checking
        if (!lowerBlock.isSolid()) {
            glm::ivec3 lowerPos = glm::ivec3{x, y - 1, z};

            // create new falling state
            WaterBlockState* newState = chunk.blockStates.createBlockState<WaterBlockState>(lowerPos);
            newState->level = 8;

            chunk.blocks[x][y - 1][z] = BlockId::BLOCK_WATER;            
            chunk.verticesOutdated = true;

            m_waterUpdates.push_back(std::make_pair(Configuration::WATER_FLOW_TIME, lowerPos));
            return;
        }
    }

    if (state->level < 7 || state->level == 8) {
        int level = state->level == 8 ? 1 : state->level + 1;

        for (int x1 = -1; x1 <= 1; x1++) {
            for (int z1 = -1; z1 <= 1; z1++) {
                if ((x1 * z1) == 0) {
                    Utility::chunk_execute(m_registry, chunk,
                                           x + x1, y, z + z1,
                                           [&](ChunkComponent& chunk, const int& cx, const int& cy, const int& cz) {
                                               Block tmp = chunk.getBlock(cx, cy, cz);
                                               if (tmp.type == BlockId::BLOCK_WATER) {
                                                   WaterBlockState* tmpState = reinterpret_cast<WaterBlockState*>(tmp.state);
                                                   if (tmpState->level > level) {
                                                       tmpState->level = level;
                                                       // updateWater(chunk, cx, cy, cz);
                                                       m_waterUpdates.push_back(std::make_pair(Configuration::WATER_FLOW_TIME, glm::vec3{chunk.chunkX * Configuration::CHUNK_SIZE + cx, cy, chunk.chunkZ * Configuration::CHUNK_SIZE + cz}));
                                                       chunk.verticesOutdated = true;
                                                   }
                                               }
                                               // not solid and no water
                                               else if (!tmp.isSolid()) {
                                                   WaterBlockState* tmp = chunk.blockStates.createBlockState<WaterBlockState>(glm::vec3{cx, cy, cz});
                                                   tmp->level = level;
                                                   chunk.blocks[cx][cy][cz] = BlockId::BLOCK_WATER;
                                                   // updateWater(chunk, cx, cy, cz);
                                                   m_waterUpdates.push_back(std::make_pair(Configuration::WATER_FLOW_TIME, glm::vec3{chunk.chunkX * Configuration::CHUNK_SIZE + cx, cy, chunk.chunkZ * Configuration::CHUNK_SIZE + cz}));
                                                   chunk.verticesOutdated = true;
                                               }
                                           });
                }
            }
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
        }
        else {
            it++;
        }
    }
}

ChunkUpdateSystem::ChunkUpdateSystem(Registry_T& registry)
    : System{registry} {
    m_blockHandle = EventDispatcher::onBlockChange.subscribe([&](const BlockChangedEvent& e) { handleBlockChanged(e); });
}
