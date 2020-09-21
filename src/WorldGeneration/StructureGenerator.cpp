#include "../../include/WorldGeneration/StructureGenerator.h"
#include "../../include/WorldGeneration/WorldGenerator.h"

#include "../../include/Configuration.h"
#include "../../include/Events/EventDispatcher.h"
#include "../../include/Systems/ChunkCreateSystem.h"
#include "../../include/Utility.h"

void StructureGenerator::generateStructure(const glm::vec2 &chunk, glm::vec3 position, const BlockCollection& structure,
                                           Block*** blocks) const {
    std::unordered_map<glm::vec2, BlockCollection, Utility::HashFunctionVec2> chunkOverflow =
        std::unordered_map<glm::vec2, BlockCollection, Utility::HashFunctionVec2>{};

    for (const auto &[localPos, blockType] : structure) {
        glm::vec3 pos = localPos + position;
        if (Utility::InChunk(pos)) {
            blocks[(int)pos.x][(int)pos.y][(int)pos.z].type = blockType;
        }
        else {            
            glm::vec3 worldPos = Utility::GetWorldCoords(chunk, pos);            

            auto [newChunk, chunkCoords] = Utility::GetChunkAndLocal(worldPos);
            if (!chunkOverflow.contains(chunk)) {
                chunkOverflow.emplace(newChunk, BlockCollection());
            }
            chunkOverflow[newChunk].emplace_back(chunkCoords, blockType);
        }
    }

    if (!chunkOverflow.empty()) {
        StructureCreatedEvent e{nullptr, chunkOverflow};
        EventDispatcher::raiseEvent(e);
    }
}

void StructureGenerator::generateTrees(const glm::vec2 &chunk, GenerationData* data) const {
    for (int cx = 0; cx < Configuration::CHUNK_SIZE; cx++) {
        for (int cz = 0; cz < Configuration::CHUNK_SIZE; cz++) {
            float random = rand() / (float)RAND_MAX;
            if (random < 0.025 && data->biomes[cx][cz] == BiomeId::BIOME_FLAT_TERRAIN) {
                bool canCreate = true;
                int surfaceHeight = m_worldGenerator->getSurfaceHeight(chunk, cx, cz);

                for (int x1 = -4; x1 <= 4; x1++) {
                    for (int y1 = -4; y1 <= 4; y1++) {
                        for (int z1 = -4; z1 <= 4; z1++) {
                            int x = cx + x1;
                            int y = surfaceHeight + 1 + y1;
                            int z = cz + z1;

                            if (Utility::InChunk(x, y, z)) {
                                if (data->blocks[x][y][z].type == BlockId::BLOCK_WOOD) {
                                    canCreate = false;
                                }
                            }
                        }
                    }
                }

                if (canCreate && data->blocks[cx][surfaceHeight - 1][cz].type == BlockId::BLOCK_GRASS) {
                    generateStructure(chunk, glm::vec3{cx, surfaceHeight, cz}, Structures::getStructure(StructureType::STRUCTURE_TREE, rand() % 3), data->blocks);
                }
            }
        }
    }
}

StructureGenerator::StructureGenerator(WorldGenerator* worldGenerator) : m_worldGenerator(worldGenerator) {
    Structures::init();
}

void StructureGenerator::generateStructures(const glm::vec2 &chunk, GenerationData* data) const {
    generateTrees(chunk, data);
}
