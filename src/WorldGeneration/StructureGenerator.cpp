#include "../../include/WorldGeneration/StructureGenerator.hpp"
#include "../../include/WorldGeneration/WorldGenerator.hpp"

#include "../../include/Configuration.hpp"
#include "../../include/Events/EventDispatcher.hpp"
#include "../../include/GameData/GameData.hpp"
#include "../../include/Systems/ChunkCreateSystem.hpp"
#include "../../include/Utility.hpp"

void StructureGenerator::fillSection(const Section& section, const glm::ivec3& position, GenerationData* data) {
    glm::ivec3 pos0 = section.position0 + position;
    glm::ivec3 pos1 = section.position1 + position;

    for (int x = pos0.x; x <= pos1.x; x++) {
        for (int y = pos0.y; y <= pos1.y; y++) {
            for (int z = pos0.z; z <= pos1.z; z++) {                
                if (Utility::inChunk(x, y, z)) {
                    // if (data->blocks[x][y][z] == BlockId::BLOCK_AIR) {
                    int sectionIndex = y / Configuration::CHUNK_SECTION_SIZE;
                    glm::ivec3 sectionPosition{x, y % Configuration::CHUNK_SECTION_SIZE, z};
                    data->sections[sectionIndex].setBlock(sectionPosition, section.block_type);
                    // }
                }
            }
        }
    }
}

std::vector<glm::ivec3> StructureGenerator::getTreePositions(const glm::ivec2& chunk) const {
    std::vector<glm::ivec3> treePositions;
    for (int cx = 0; cx < Configuration::CHUNK_SIZE; cx++) {
        for (int cz = 0; cz < Configuration::CHUNK_SIZE; cz++) {
            if (m_worldGenerator->getBiome(chunk, cx, cz) == BiomeId::BIOME_FLAT_TERRAIN) {
                int x = cx + chunk.x * Configuration::CHUNK_SIZE;
                int z = cz + chunk.y * Configuration::CHUNK_SIZE;
                if (m_treeNoise.getValue(x, z)) {
                    int y = m_worldGenerator->getSurfaceHeight(chunk, cx, cz) + 1;
                    treePositions.emplace_back(cx, y, cz);
                }
            }
        }
    }

    return treePositions;
}

// TODO: remove flying trees
void StructureGenerator::generateTrees(const glm::ivec2& chunk, GenerationData* data) const {
    const Structure& treeStructure = GameData::getStructure(StructureId::STRUCTURE_TREE, 0);

    for (int cxo = -1; cxo <= 1; cxo++) {
        for (int czo = -1; czo <= 1; czo++) {
            glm::ivec2 chunkOffset{cxo, czo};
            std::vector<glm::ivec3> trees = getTreePositions(chunk + chunkOffset);
            for (auto& tree : trees) {
                glm::ivec3 treePos = tree + glm::ivec3{cxo * Configuration::CHUNK_SIZE, 0, czo * Configuration::CHUNK_SIZE};                
                for (const auto& section : treeStructure.sections) {
                    fillSection(section, treePos, data);
                }
            }
        }
    }
}

StructureGenerator::StructureGenerator(WorldGenerator* worldGenerator)
    : m_worldGenerator(worldGenerator) {
    m_treeNoise.setRadius(4);
}

void StructureGenerator::generateStructures(const glm::vec2& chunk, GenerationData* data) const {
    generateTrees((glm::ivec2)chunk, data);
}
