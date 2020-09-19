#include "../../include/WorldGeneration/ForrestGenerator.h"

#include "../../include/Configuration.h"

void ForrestGenerator::setBlock(glm::vec2 chunk, int cx, int cy, int cz, Block*** blocks, BlockType type) {
    if (Utility::InChunk(cx, cy, cz)) {
        blocks[cx][cy][cz] = Block(type);
    }
    else {
        glm::vec3 worldCoords = (float)Configuration::CHUNK_SIZE * glm::vec3{chunk.x, 0, chunk.y} + glm::vec3{cx, cy, cz};

        auto [realChunk, chunkCoords] = Utility::GetChunkAndLocal(worldCoords);

        if (!generationData.contains(realChunk)) {
            generationData.emplace(chunk, std::vector<std::tuple<glm::vec3, BlockType>>());
        }

        generationData[realChunk].emplace_back(chunkCoords, type);
    }
}

void ForrestGenerator::generateTrees(glm::vec2 chunk, Block*** blocks, BiomeID** biomes) {
    if (generationData.contains(chunk)) {
        for (auto [position, type] : generationData[chunk]) {
            blocks[(int)position.x][(int)position.y][(int)position.z] = Block(type);
        }

        generationData.erase(chunk);
    }

    for (int cx = 0; cx < Configuration::CHUNK_SIZE; cx++) {
        for (int cz = 0; cz < Configuration::CHUNK_SIZE; cz++) {
            if (biomes[cx][cz] != BiomeID::BIOME_FLAT_TERRAIN)
                continue;

            int terrainHeight = 0;

            for (int y = Configuration::CHUNK_HEIGHT - 1; y >= 0; y--) {
                if (blocks[cx][y][cz].type == BlockType::BLOCK_GRASS) {
                    terrainHeight = y;
                    break;
                }
                else if (blocks[cx][y][cz].type != BlockType::BLOCK_AIR) {
                    terrainHeight = -1;
                    break;
                }
            }

            if (terrainHeight < 0)
                continue;

            // TODO: use noise module
            float random = rand() / (float)RAND_MAX;
            int treeHeight = rand() % 2 + 4;
            if (random < threshold) {
                // check if tree could be created
                bool canCreate = true;

                for (int x1 = -4; x1 <= 4; x1++) {
                    for (int y1 = -4; y1 <= 4; y1++) {
                        for (int z1 = -4; z1 <= 4; z1++) {
                            if (x1 * x1 + y1 * y1 + z1 * z1 <= 64) {
                                int x = x1 + cx;
                                int y = y1 + terrainHeight + 1;
                                int z = z1 + cz;
                                if (Utility::InChunk(x, y, z)) {
                                    if (blocks[x][y][z].type == BlockType::BLOCK_WOOD) {
                                        canCreate = false;
                                    }
                                }
                            }
                        }
                    }
                }

                // create tree
                if (canCreate) {

                    int heightOffset = treeHeight - 2;

                    for (int y1 = 0; y1 < treeHeight + 2; y1++) {
                        if (y1 < heightOffset)
                            continue;

                        if (y1 < treeHeight) {
                            for (int x1 = -2; x1 <= 2; x1++) {
                                for (int z1 = -2; z1 <= 2; z1++) {
                                    int x = cx + x1;
                                    int y = y1 + terrainHeight + 1;
                                    int z = cz + z1;

                                    setBlock(chunk, x, y, z, blocks);
                                }
                            }
                        }
                        else {
                            for (int x1 = -1; x1 <= 1; x1++) {
                                for (int z1 = -1; z1 <= 1; z1++) {
                                    if (!(std::abs(x1) == 1 && std::abs(z1) == 1)) {
                                        int x = cx + x1;
                                        int y = y1 + terrainHeight + 1;
                                        int z = cz + z1;

                                        setBlock(chunk, x, y, z, blocks);
                                    }
                                }
                            }
                        }
                    }

                    for (int y = 0; y < treeHeight; y++) {
                        blocks[cx][y + terrainHeight + 1][cz] = Block(BlockType::BLOCK_WOOD);
                    }
                }
            }
        }
    }
}