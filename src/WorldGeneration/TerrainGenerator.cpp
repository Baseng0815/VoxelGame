#include "../../include/WorldGeneration/TerrainGenerator.h"
#include "../../include/Configuration.h"
#include "../../include/Block.h"

void TerrainGenerator::_init() {
    
}

void TerrainGenerator::generateTerrain(glm::vec2 chunk, Block*** blocks) {
    for(int x = 0; x < Configuration::CHUNK_SIZE; x++)
        for(int z = 0; z < Configuration::CHUNK_SIZE; z++) {
            blocks[x][0][z] = Block(BlockType::BLOCK_BRICKS);

            for(int y = 1; y < 4; y++){
                blocks[x][y][z] = Block(BlockType::BLOCK_DIRT);
            }

            blocks[x][4][z] = Block(BlockType::BLOCK_GRASS);
    }
}

TerrainGenerator::TerrainGenerator() {
    _init();
}