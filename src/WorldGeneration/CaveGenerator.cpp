#include "../../include/WorldGeneration/CaveGenerator.hpp"

#include "../../include/GameData/Block.hpp"
#include "../../include/Configuration.hpp"

CaveGenerator::CaveGenerator() {
    caveNoise.SetFrequency(0.02f);
    caveNoise.SetOctaveCount(2);
    caveNoise.SetLacunarity(1.25f);
}

void CaveGenerator::generateChunk(glm::vec2 chunkPos, GenerationData* data) const {
    int bricks = 1;
    int air = 0;
    int water = 3;
    int lava = data->blockData.size();

    data->blockData.push_back(BlockId::BLOCK_LAVA);

    for (int x = 0; x < Configuration::CHUNK_SIZE; x++) {
        for (int y = 0; y < Configuration::CHUNK_HEIGHT; y++) {
            for (int z = 0; z < Configuration::CHUNK_SIZE; z++) {
                if (data->blocks[x][y][z] != bricks && (data->blocks[x][y][z] & 0x0000FFFF) != water) {
                    if (caveNoise.GetValue(x + chunkPos.x * Configuration::CHUNK_SIZE, y,
                                            z + chunkPos.y * Configuration::CHUNK_SIZE) > threshold) {
                        if(y > 10) {
                            data->blocks[x][y][z] = 0;
                        }
                        else {
                            data->blocks[x][y][z] = 1 << 16 | lava;
                        }
                    }
                }
            }
        }
    }
}
