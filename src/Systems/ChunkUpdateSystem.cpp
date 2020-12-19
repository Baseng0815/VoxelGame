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
    auto [chunkCoords, position] = Utility::GetChunkAndLocal(e.position);
    ChunkComponent& chunk = World::getChunk(m_registry, chunkCoords);

    for (int x = -1; x <= 1; x++) {
        for (int y = 0; y <= 1; y++) {
            for (int z = -1; z <= 1; z++) {
                if ((x * z) == 0) {
                    int cx = position.x + x;
                    int cy = position.y + y;
                    int cz = position.z + z;

                    Utility::chunk_execute(m_registry, chunk, cx, cy, cz,
                                           [&](ChunkComponent& otherChunk, const int& x, const int& y, const int& z) {
                                               BlockId block = otherChunk.blocks[x][y][z];
                                               if (block == BlockId::BLOCK_WATER) {
                                                   updateWater(otherChunk, x, y, z);
                                               }
                                           });
                }
            }
        }
    }

    chunk.verticesOutdated = true;
}

void ChunkUpdateSystem::updateWater(ChunkComponent& chunk, const int& x, const int& y, const int& z) {
    Block block = chunk.getBlock(x, y, z);
    WaterBlockState* state = reinterpret_cast<WaterBlockState*>(block.state);

    if (y > 1) {
        Block lowerBlock = chunk.getBlock(x, y - 1, z);
        // TODO: Range checking
        if (!lowerBlock.isSolid()) {
            glm::vec3 lowerPos = glm::vec3{x, y - 1, z};

            // create new falling state            
            WaterBlockState* newState = chunk.blockStates.createBlockState<WaterBlockState>(lowerPos);
            newState->level = 8;

            chunk.blocks[x][y - 1][z] = BlockId::BLOCK_WATER;
            updateWater(chunk, x, y - 1, z);
            chunk.verticesOutdated = true;
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
                                                       updateWater(chunk, cx, cy, cz);
                                                       chunk.verticesOutdated = true;
                                                   }
                                               }
                                               // not solid and no water
                                               else if (!tmp.isSolid()) {
                                                   WaterBlockState* tmp = chunk.blockStates.createBlockState<WaterBlockState>(glm::vec3{cx, cy, cz});
                                                   tmp->level = level;
                                                   chunk.blocks[cx][cy][cz] = BlockId::BLOCK_WATER;
                                                   updateWater(chunk, cx, cy, cz);
                                                   chunk.verticesOutdated = true;
                                               }
                                           });
                }
            }
        }
    }
}

void ChunkUpdateSystem::_update(int dt) {
}

ChunkUpdateSystem::ChunkUpdateSystem(Registry_T& registry)
    : System{registry} {
    m_blockHandle = EventDispatcher::onBlockChange.subscribe([&](const BlockChangedEvent& e) { handleBlockChanged(e); });
}
