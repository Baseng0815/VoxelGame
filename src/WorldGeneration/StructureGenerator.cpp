#include "../../include/WorldGeneration/StructureGenerator.h"

#include "../../include/Systems/ChunkCreateSystem.h"
#include "../../include/Configuration.h"
#include "../../include/Utility.h"
#include "../../include/Events/EventDispatcher.h"

void StructureGenerator::generateStructure(glm::vec2 chunk, glm::vec3 position, const BlockCollection& structure,
	Block*** blocks) const {
	std::unordered_map<glm::vec2, BlockCollection, Utility::HashFunctionVec2> chunkOverflow = std::unordered_map<glm::vec2, BlockCollection, Utility::HashFunctionVec2>{};

	for (auto [localPos, blockType] : structure) {
		glm::vec3 pos = localPos + position;
		if (Utility::InChunk(pos)) {
			blocks[(int)pos.x][(int)pos.y][(int)pos.z] = blockType;
		}
		else {
			glm::vec3 worldPos = Utility::GetWorldCoords(chunk, pos);
			auto [newChunk, chunkCoords] = Utility::GetChunkAndLocal(worldPos);
			if (chunkOverflow.contains(chunk)) {
				chunkOverflow[newChunk].emplace_back(chunkCoords, blockType);
			}
		}
	}

	if (!chunkOverflow.empty()) {
		StructureCreatedEvent e{ nullptr, chunkOverflow };
		EventDispatcher::raiseEvent(e);
	}
}

StructureGenerator::StructureGenerator() {
	Structures::init();
}

void StructureGenerator::generateStructures(glm::vec2 chunk, GenerationData* data) const {
	int x = 1;
	int z = 1;

	if (data->biomes[x][z] == BiomeID::BIOME_FLAT_TERRAIN) {
		int y = Configuration::CHUNK_HEIGHT;

		for (int i = y - 1; i >= 0; i--) {
			if (data->blocks[x][i][z].type == BlockType::BLOCK_GRASS) {
				y = i;
				break;
			}
		}

		if (y > 0 && y < Configuration::CHUNK_HEIGHT) {
			int variation = rand() % 3;
			auto structure = Structures::getStructure(StructureType::STRUCTURE_TREE, variation);

			generateStructure(chunk, glm::vec3{ x, y, z }, structure, data->blocks);
		}
	}
}
