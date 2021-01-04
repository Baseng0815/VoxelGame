#include "../../include/WorldGeneration/CaveGenerator.hpp"

#include "../../include/Configuration.hpp"
#include "../../include/GameData/Block.hpp"

CaveGenerator::CaveGenerator() {
    caveNoise.SetFrequency(0.02f);
    caveNoise.SetOctaveCount(2);
    caveNoise.SetLacunarity(1.25f);
}

void CaveGenerator::generateChunk(glm::vec2 chunkPos, GenerationData* data) const {
    for (int x = 0; x < Configuration::CHUNK_SIZE; x++) {
        for (int y = 0; y < Configuration::CHUNK_HEIGHT; y++) {
            for (int z = 0; z < Configuration::CHUNK_SIZE; z++) {
                if (caveNoise.GetValue(x + chunkPos.x * Configuration::CHUNK_SIZE, y,
                                       z + chunkPos.y * Configuration::CHUNK_SIZE) > threshold) {
                    int sectionIndex = y / Configuration::CHUNK_SECTION_SIZE;
                    glm::ivec3 position{x, y % Configuration::CHUNK_SECTION_SIZE, z};
                    BlockId block = data->sections[sectionIndex].getBlock(position);
                    if (block != BlockId::BLOCK_BEDROCK && block != BlockId::BLOCK_WATER) {
                        data->sections[sectionIndex].setBlock(position, y < 10 ? BlockId::BLOCK_LAVA : BlockId::BLOCK_AIR);
                    }
                }
            }
        }
    }
}
