#pragma once
#include "../GameData/BiomeIds.hpp"
#include "../GameData/Block.hpp"
#include "../GameData/BlockStates/BlockStateContainer.hpp"
#include "../GameData/ChunkSection.hpp"

#include <entt/entt.hpp>

struct GenerationData {
    entt::entity entity;
    
    BlockStateContainer stateData;    
    BiomeId** biomes;

    ChunkSection* sections;
};