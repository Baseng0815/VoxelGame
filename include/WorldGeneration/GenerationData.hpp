#pragma once
#include <entt/entt.hpp>
#include "../GameData/Block.hpp"
#include "../GameData/BlockStates/BlockState.hpp"
#include "../GameData/BiomeIds.hpp"

struct GenerationData {
    entt::entity entity;
    std::vector<BlockId> blockData;
    std::vector<BlockState*> stateData;
    int*** blocks;
    BiomeId** biomes;
};