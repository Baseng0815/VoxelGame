#pragma once

#include "CaveGenerator.hpp"
#include "TerrainHeightGenerator.hpp"
#include "TerrainGenerator.hpp"

#include "../GameData/Block.hpp"

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

	void generateOres(BiomeId** biomes, Block*** blocks) const;	

    public:
        WorldGenerator(WorldType type);
        WorldGenerator(const WorldGenerator&);	

        void init(WorldType worldType);

	void generate(glm::vec2 position, GenerationData* data);

	int getSurfaceHeight(glm::vec2 chunk, int x, int z) const;
};

