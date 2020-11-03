#include "../../include/Systems/ChunkUpdateSystem.hpp"

#include "../../include/Components/ChunkComponent.hpp"
#include "../../include/GameData/Block.hpp"
#include "../../include/GameData/BlockStates/WaterBlockState.hpp"
#include "../../include/World.hpp"

#include "../../include/Configuration.hpp"

#include <vector>

void ChunkUpdateSystem::updateFluids(ChunkComponent& chunk, int dt) {
    int waterId = chunk.getBlockIndex(BlockId::BLOCK_WATER);

    for (int x = 0; x < Configuration::CHUNK_SIZE; x++) {
        for (int y = 0; y < Configuration::CHUNK_HEIGHT; y++) {
            for (int z = 0; z < Configuration::CHUNK_SIZE; z++) {
                Block block = chunk.getBlock(x, y, z);
                if (!block.isSolid()) {
                    if (Utility::inChunk(x - 1, y, z)) {
                        if (chunk.blocks[x - 1][y][z] == waterId) {
                            WaterBlockState& waterState = chunk.blockStates.getState<WaterBlockState>(glm::vec3{x - 1, y, z});
                            updateWater(chunk, block, waterState);
                        }
                    }
                    // else {
                    //     ChunkComponent& ch = World::getChunk(m_registry, glm::vec2{chunk.chunkX - 1, chunk.chunkZ});
                    // }

                    if (Utility::inChunk(x + 1, y, z)) {
                        if (chunk.blocks[x + 1][y][z] == waterId) {
                            WaterBlockState& waterState = chunk.blockStates.getState<WaterBlockState>(glm::vec3{x + 1, y, z});
                            updateWater(chunk, block, waterState);
                        }
                    }

                    if (Utility::inChunk(x, y + 1, z)) {
                        if (chunk.blocks[x][y + 1][z] == waterId) {
                            WaterBlockState& waterState = chunk.blockStates.getState<WaterBlockState>(glm::vec3{x, y + 1, z});
                            updateWater(chunk, block, waterState, true);
                        }
                    }

                    if (Utility::inChunk(x, y, z - 1)) {
                        if (chunk.blocks[x][y][z - 1] == waterId) {
                            WaterBlockState& waterState = chunk.blockStates.getState<WaterBlockState>(glm::vec3{x, y, z - 1});
                            updateWater(chunk, block, waterState);
                        }
                    }

                    if (Utility::inChunk(x, y, z + 1)) {
                        if (chunk.blocks[x][y][z + 1] == waterId) {
                            WaterBlockState& waterState = chunk.blockStates.getState<WaterBlockState>(glm::vec3{x, y, z + 1});
                            updateWater(chunk, block, waterState);
                        }
                    }
                }
            }
        }
    }
}

void ChunkUpdateSystem::updateWater(ChunkComponent& chunk, Block& block, WaterBlockState& waterState, bool waterOnTop) {
    glm::vec3 position = Utility::GetChunkCoords(block.position);
    int x = position.x, y = position.y, z = position.z;

    if (waterOnTop) {
        chunk.blocks[x][y][z] = chunk.getBlockIndex(BlockId::BLOCK_WATER);
        // chunk.verticesOutdated = true;
    }
    else {
        int level = waterState.level + 1;

        if (block.type == BlockId::BLOCK_WATER) {
            WaterBlockState* oldState = reinterpret_cast<WaterBlockState*>(block.state);
            if (oldState->level <= level) {
                return;
            }
        }
        
        if (level <= 7) {
            chunk.blockStates.deleteBlockState<WaterBlockState>(position);

            WaterBlockState& newState = chunk.blockStates.createBlockState<WaterBlockState>(position);
            newState.falling = false;
            newState.level = level;
            chunk.blocks[x][y][z] = chunk.getBlockIndex(BlockId::BLOCK_WATER);
            // chunk.verticesOutdated = true;
        }
    }
}

void ChunkUpdateSystem::_update(int dt) {
    m_registry.view<ChunkComponent>().each(
        [&](ChunkComponent& chunk) {
            if (chunk.needsUpdate) {
                chunk.needsUpdate = false;
                updateFluids(chunk, dt);

                // chunk.verticesOutdated = true;
            }
        });
}

ChunkUpdateSystem::ChunkUpdateSystem(Registry_T& registry)
    : System{registry, 0} {
}