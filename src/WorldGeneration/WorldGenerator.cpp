// in der .cpp kann man einbinden, was man will, da entsteht kein Problem
#include "../../include/WorldGeneration/WorldGenerator.h"
#include "../../include/WorldGeneration/noiseutils.h"
#include "../../include/WorldGeneration/Biome.h"
#include "../../include/Block.h"
#include "../../include/BlockData.h"
#include "../../include/Configuration.h"
#include "../../include/Utility.h"
#include "../../include/Systems/ChunkCreateSystem.h"

#include <iostream>

using namespace noise::utils;
using namespace noise::model;

WorldGenerator::WorldGenerator(WorldType worldType) {
    m_type = worldType;
}

void WorldGenerator::generateOres(BiomeID** biomes, Block*** blocks) const {
    for (BlockType block = BlockType::BLOCK_ORE_GOLD; block <= BlockType::BLOCK_ORE_COAL; ) {
        BlockData blockData = Configuration::getBlockData(block);

        for (int c = 0; c < blockData.oreData.generationCounts; c++) {
            int x = rand() % Configuration::CHUNK_SIZE;
            int y = rand() % (blockData.oreData.maxHeight - blockData.oreData.minHeight) + blockData.oreData.minHeight;
            int z = rand() % Configuration::CHUNK_SIZE;
            glm::vec3 orePos = glm::vec3(x, y, z);

            double a = pow(3 * blockData.oreData.size / (16 * PI), 1 / 3.0);

            //		x^2		y^2		z^2
            // 1 = ----- + ----- + -----
            //		a^2		a^2		4a^2

            for(int x1 = -a; x1 <= a; x1++)
                for (int z1 = -2 * a; z1 <= 2 * a; z1++) {
                    double fSquared = pow(a, 2) - pow(x1, 2) - (pow(z1, 2) / pow(a, 2));					

                    if (fSquared >= 0) {
                        double ymin = -pow(fSquared, 0.5);
                        double ymax = -ymin;

                        for (int y1 = ymin; y1 <= ymax; y1++) {
                            glm::vec3 pos = orePos + glm::vec3(x1, y1, z1);	

                            if(pos.x < 0 || pos.x >= Configuration::CHUNK_SIZE || pos.y < 0 || pos.y >= Configuration::CHUNK_HEIGHT || pos.z < 0 || pos.z >= Configuration::CHUNK_SIZE)						
                                continue;

                            if (blocks[(int)pos.x][(int)pos.y][(int)pos.z].type == BlockType::BLOCK_STONE) {
                                blocks[(int)pos.x][(int)pos.y][(int)pos.z] = Block(block);
                            }
                        }
                    }
                }
        }

        block = (BlockType)((int)block + 1);
    }
}

void WorldGenerator::generate(glm::vec2 position, GenerationData* data) const {
    int** heightMap = new int*[Configuration::CHUNK_SIZE];
    for(int i = 0; i < Configuration::CHUNK_SIZE; i++) heightMap[i] = new int[Configuration::CHUNK_SIZE];

    m_heightGenerator.generateChunkHeight(position, heightMap, data->biomes);
    m_terrainGenerator.createBlocks(data->blocks, heightMap, data->biomes);        

    m_caveGenerator.generateChunk(position, data->blocks);
}
