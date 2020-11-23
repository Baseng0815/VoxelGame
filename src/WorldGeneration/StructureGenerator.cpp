#include "../../include/WorldGeneration/StructureGenerator.hpp"
#include "../../include/WorldGeneration/WorldGenerator.hpp"

#include "../../include/Configuration.hpp"
#include "../../include/Events/EventDispatcher.hpp"
#include "../../include/Systems/ChunkCreateSystem.hpp"
#include "../../include/Utility.hpp"

void StructureGenerator::generateStructure(const glm::vec2& chunk, glm::vec3 position, const BlockCollection& structure,
                                           Block*** blocks) const {
    std::unordered_map<glm::vec2, BlockCollection, std::hash<glm::vec2>> chunkOverflow =
        std::unordered_map<glm::vec2, BlockCollection, std::hash<glm::vec2>>{};

    for (const auto& [localPos, blockType] : structure) {
        glm::vec3 pos = localPos + position;
        if (Utility::inChunk(pos)) {
            blocks[(int)pos.x][(int)pos.y][(int)pos.z].type = blockType;
        }
        else {
            glm::vec3 worldPos = Utility::getWorldCoords(chunk, pos);

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

std::vector<glm::ivec2> StructureGenerator::getTreePositions(int chunkX, int chunkZ) const {
    std::vector<glm::ivec2> treePositions;
    for (int cx = 0; cx < Configuration::CHUNK_SIZE; cx++) {
        for (int cz = 0; cz < Configuration::CHUNK_SIZE; cz++) {
            int x = cx + chunkX * Configuration::CHUNK_SIZE;
            int z = cz + chunkZ * Configuration::CHUNK_SIZE;
            if (m_treeNoise.getValue(x, z)) {
                treePositions.emplace_back(x, z);
            }
        }
    }

    return treePositions;
}

void StructureGenerator::generateTrees(const glm::vec2& chunk, GenerationData* data) const {
    std::vector<glm::ivec2> treePositions;
    for (int x = chunk.x - 1; x <= chunk.x + 1; x++) {
        for (int y = chunk.y - 1; y <= chunk.y + 1; y++) {
            if (x == chunk.x && y == chunk.y) {
                continue;
            }

            std::vector<glm::ivec2> trees = getTreePositions(x, y);
            treePositions.insert(treePositions.begin() + treePositions.size(), trees.begin(), trees.end());
        }
    }
}

StructureGenerator::StructureGenerator(WorldGenerator* worldGenerator)
    : m_worldGenerator(worldGenerator) {
    Structures::init();
}

void StructureGenerator::generateStructures(const glm::vec2& chunk, GenerationData* data) const {
    generateTrees(chunk, data);
}
