#include "../../include/WorldGeneration/Structures.h"

bool StructureId::operator==(const StructureId& other) const {
    return type == other.type && variation == other.variation;
}

long StructureIdHash::operator()(const StructureId& id) const {
    return std::hash<int>{}((int)id.type) ^ (std::hash<int>{}(id.variation) << 1);
}

bool Structures::initialized = false;
std::unordered_map<StructureId, BlockCollection, StructureIdHash> Structures::structures =
    std::unordered_map<StructureId, BlockCollection, StructureIdHash>{};

BlockCollection Structures::getTreeBlocks(int height) {
    BlockCollection blocks = BlockCollection{};

    for (int i = 0; i < height; i++) {
        blocks.emplace_back(glm::vec3{0, i, 0}, BlockType::BLOCK_WOOD);
    }

    int heightOffset = height - 2;
    for (int y = heightOffset; y <= height + 1; y++) {
        if (y < height) {
            for (int x = -2; x <= 2; x++) {
                for (int z = -2; z <= 2; z++) {
                    if(x != 0 || z != 0)
                        blocks.emplace_back(glm::vec3{x, y, z}, BlockType::BLOCK_LEAVES);
                }
            }
        }
        else {
            blocks.emplace_back(glm::vec3{0, y, -1}, BlockType::BLOCK_LEAVES);
            blocks.emplace_back(glm::vec3{-1, y, 0}, BlockType::BLOCK_LEAVES);
            if (y >= height)
                blocks.emplace_back(glm::vec3{0, y, 0}, BlockType::BLOCK_LEAVES);
            blocks.emplace_back(glm::vec3{1, y, 0}, BlockType::BLOCK_LEAVES);
            blocks.emplace_back(glm::vec3{0, y, 1}, BlockType::BLOCK_LEAVES);
        }
    }

    return blocks;
}

void Structures::init() {
    if (!initialized) {
        structures.emplace(StructureId{StructureType::STRUCTURE_TREE, 0}, getTreeBlocks(3));
        structures.emplace(StructureId{StructureType::STRUCTURE_TREE, 1}, getTreeBlocks(4));
        structures.emplace(StructureId{StructureType::STRUCTURE_TREE, 2}, getTreeBlocks(5));

        initialized = true;
    }
}

const BlockCollection& Structures::getStructure(StructureType type, int variation) {
    StructureId id{type, variation};

    return structures.at(id);
}