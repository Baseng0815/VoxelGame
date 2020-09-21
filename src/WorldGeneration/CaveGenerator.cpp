#include "../../include/WorldGeneration/CaveGenerator.h"

#include "../../include/GameData/Block.h"
#include "../../include/Configuration.h"

CaveGenerator::CaveGenerator() {
    caveNoise.SetFrequency(0.02f);
    caveNoise.SetOctaveCount(2);
    caveNoise.SetLacunarity(1.25f);
}

void CaveGenerator::generateChunk(glm::vec2 chunkPos, Block*** blocks) const {
    for (int x = 0; x < Configuration::CHUNK_SIZE; x++) {
        for (int y = 0; y < Configuration::CHUNK_HEIGHT; y++) {
            for (int z = 0; z < Configuration::CHUNK_SIZE; z++) {
                if (blocks[x][y][z].type != BlockId::BLOCK_BRICKS && blocks[x][y][z].type != BlockId::BLOCK_WATER) {
                    if (caveNoise.GetValue(x + chunkPos.x * Configuration::CHUNK_SIZE, y,
                                            z + chunkPos.y * Configuration::CHUNK_SIZE) > threshold) {
                        if(y > 10) {
                            blocks[x][y][z] = Block {BlockId::BLOCK_AIR};
                        }
                        else {
                            blocks[x][y][z] = Block{BlockId::BLOCK_LAVA};
                        }
                    }
                }
            }
        }
    }
}
