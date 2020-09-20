#pragma once

#include "CaveGenerator.h"
#include "TerrainHeightGenerator.h"
#include "TerrainGenerator.h"
#include "ForrestGenerator.h"

#include "../GameData/Block.h"

#include <glm/glm.hpp>
#include <unordered_map>
#include <noise/noise.h>

using namespace noise;
using namespace noise::module;

struct ChunkComponent;
struct GenerationData;

enum class WorldType : char {
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
        ForrestGenerator m_forrestGenerator;

        void generateOres(BiomeId** biomes, Block*** blocks) const;	

    public:
        WorldGenerator(WorldType type);
        WorldGenerator(const WorldGenerator&);	

        void init(WorldType worldType);

        void generate(glm::vec2 position, GenerationData* data);
};

