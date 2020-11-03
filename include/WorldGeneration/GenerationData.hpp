#pragma once
#include "../GameData/BiomeIds.hpp"
#include "../GameData/Block.hpp"
#include "../GameData/BlockStates/BlockStateContainer.hpp"

#include <entt/entt.hpp>

struct GenerationData {
    entt::entity entity;
    std::vector<BlockId> blockData;
    BlockStateContainer stateData;
    short*** blocks;
    BiomeId** biomes;
};