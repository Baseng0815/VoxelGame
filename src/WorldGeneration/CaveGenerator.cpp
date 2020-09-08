#include "../../include/WorldGeneration/CaveGenerator.h"

#include "../../include/Block.h"
#include "../../include/Components/WorldComponent.h"
#include "../../include/Configuration.h"
#include "../../include/WorldGeneration/PerlinWorm.h"

#include <iostream>

int CaveGenerator::getValue(int x, int y, int z) const {
    float noiseValue = noise.GetValue(x, y, z);

    return (noiseValue < density) ? 1 : 0;
}

void CaveGenerator::generate(glm::vec2 chunk, Block*** blocks) {
    for (int x = 0; x < 3 * Configuration::CHUNK_SIZE; x++) {
        for (int y = 0; y < Configuration::CHUNK_HEIGHT; y++) {
            for (int z = 0; z < 3 * Configuration::CHUNK_SIZE; z++) {
                cache[x][y][z] = getValue(x, y, z);
            }
        }
    }
}
