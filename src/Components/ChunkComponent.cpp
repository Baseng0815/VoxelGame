#include "../../include/Components/ChunkComponent.hpp"

#include "../../include/Configuration.hpp"
#include "../../include/GameData/BlockStates/WaterBlockState.hpp"
#include "../../include/Utility.hpp"

int ChunkComponent::getBlockIndex(const BlockId& type) const {
    int result = -1;
    int index = 0;

    while (result == -1 && index < blockData.size()) {
        if (blockData[index] == type) {
            result = index;
        }
        index++;
    }

    return result;
}

void ChunkComponent::setBlock(int x, int y, int z, const Block& block) {
    // type
    int typeIndex = getBlockIndex(block.type);
    glm::vec3 blockPosition{x, y, z};

    if (typeIndex == -1) {
        typeIndex = blockData.size();
        blockData.emplace_back(block.type);
    }

    // state
    // if needed delete old state and create new state
    switch (block.type) {
        case BlockId::BLOCK_WATER:
            blockStates.deleteBlockState<WaterBlockState>(blockPosition);
            blockStates.createBlockState<WaterBlockState>(blockPosition);
            break;
    }

    blocks[x][y][z] = typeIndex;
}

void ChunkComponent::setBlock(const glm::vec3& position, const Block& block) {
    // type
    int typeIndex = getBlockIndex(block.type);

    if (typeIndex == -1) {
        typeIndex = blockData.size();
        blockData.emplace_back(block.type);
    }

    // state
    // if needed delete old state and create new state
    switch (block.type) {
        case BlockId::BLOCK_WATER:
            blockStates.deleteBlockState<WaterBlockState>(position);
            blockStates.createBlockState<WaterBlockState>(position);
            break;
    }

    blocks[(int)position.x][(int)position.y][(int)position.z] = typeIndex;
}

const Block ChunkComponent::getBlock(int x, int y, int z) const {
    if (inRange(x, 0, Configuration::CHUNK_SIZE) && inRange(y, 0, Configuration::CHUNK_HEIGHT) && inRange(z, 0, Configuration::CHUNK_SIZE)) {
        int blockTypeId = blocks[x][y][z];

        BlockId type = blockData[blockTypeId];        

        return Block{Utility::GetWorldCoords(chunkX, chunkZ, x, y, z), type};
    }
    else {
        return Block{};
    }
}

const Block ChunkComponent::getBlock(const glm::vec3& position) const {
    return getBlock(position.x, position.y, position.z);
}

Block ChunkComponent::getBlock(int x, int y, int z) {
    if (inRange(x, 0, Configuration::CHUNK_SIZE) && inRange(y, 0, Configuration::CHUNK_HEIGHT) && inRange(z, 0, Configuration::CHUNK_SIZE)) {
        int blockTypeId = blocks[x][y][z];

        BlockId type = blockData[blockTypeId];
        Block block;
        switch (type) {
            case BlockId::BLOCK_WATER: {
                WaterBlockState* state = blockStates.getState<WaterBlockState>(glm::vec3{x, y, z});
                block = Block{Utility::GetWorldCoords(chunkX, chunkZ, x, y, z), type, state};
            } break;
            default:
                block = Block{Utility::GetWorldCoords(chunkX, chunkZ, x, y, z), type, nullptr};
                break;
        }

        return block;
    }
    else {
        return Block{};
    }
}

Block ChunkComponent::getBlock(const glm::vec3& position) {
    return getBlock(position.x, position.y, position.z);
}
