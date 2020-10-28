#include "../../include/Systems/ChunkUpdateSystem.hpp"

#include "../../include/Components/ChunkComponent.hpp"
#include "../../include/GameData/BlockStates/BlockStates.hpp"
#include "../../include/GameData/BlockStates/WaterBlockState.hpp"

void ChunkUpdateSystem::updateFluids(ChunkComponent& chunk, int dt) {
}

void ChunkUpdateSystem::_update(int dt) {
    m_registry.view<ChunkComponent>().each(
        [&](ChunkComponent& chunk) {
            updateFluids(chunk, dt);
        });
}

ChunkUpdateSystem::ChunkUpdateSystem(Registry_T registry)
    : System{registry, 0} {
}