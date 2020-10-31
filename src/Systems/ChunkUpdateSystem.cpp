#include "../../include/Systems/ChunkUpdateSystem.hpp"

#include "../../include/GameData/Block.hpp"
#include "../../include/Components/ChunkComponent.hpp"
#include "../../include/Components/BlockStateComponent.hpp"
#include "../../include/GameData/BlockStates/WaterBlockState.hpp"
#include "../../include/World.hpp"

#include "../../include/Configuration.hpp"

void ChunkUpdateSystem::updateFluids(ChunkComponent& chunk, int dt) {
    for (int x = 0; x < Configuration::CHUNK_SIZE; x++) {
        for (int y = 0; y < Configuration::CHUNK_HEIGHT; y++) {
            for (int z = 0; z < Configuration::CHUNK_SIZE; z++) {
                try {
                    glm::vec3 globalPosition = Utility::GetWorldCoords(glm::vec2{chunk.chunkX, chunk.chunkZ}, glm::vec3{x, y, z});
                    // TODO: Save blockstates per chunk
                    BlockStateComponent& component = World::getBlockState(m_registry, globalPosition);

                    if(!component.state.outdated)
                        continue;

                    if (chunk.getBlock(x, y, z).type == BlockId::BLOCK_WATER) {                    
                    }
                }
                catch (std::exception) {
                    continue;
                }
            }
        }
    }
}

void ChunkUpdateSystem::_update(int dt) {
    m_registry.view<ChunkComponent>().each(
        [&](ChunkComponent& chunk) {
            updateFluids(chunk, dt);
        });
}

ChunkUpdateSystem::ChunkUpdateSystem(Registry_T& registry)
    : System{registry, 0} {
}