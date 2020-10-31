#pragma once

#include "CaveGenerator.hpp"
#include "TerrainGenerator.hpp"
#include "TerrainHeightGenerator.hpp"

#include "../GameData/Block.hpp"
#include "../GameData/BlockStates/BlockState.hpp"

#include <glm/glm.hpp>
#include <noise/noise.h>
#include <unordered_map>
#include <entt/entt.hpp>

using namespace noise;
using namespace noise::module;

struct ChunkComponent;

enum class WorldType : char
{
    WORLD_FLAT,
    WORLD_NORMAL
};

class WorldGenerator {
  private:
    WorldType m_type = WorldType::WORLD_FLAT;

    //BiomeGenerator m_biomeGenerator;
    CaveGenerator m_caveGenerator;
    TerrainHeightGenerator m_heightGenerator;
    TerrainGenerator m_terrainGenerator;

    void generateOres(BiomeId** biomes, Block*** blocks) const;

  public:
    WorldGenerator(WorldType type);
    WorldGenerator(const WorldGenerator&);

    void init(WorldType worldType);

    void generate(glm::vec2 position, GenerationData* data);

    int getSurfaceHeight(glm::vec2 chunk, int x, int z) const;
};
