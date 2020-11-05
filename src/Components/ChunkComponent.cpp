#include "../../include/Components/ChunkComponent.hpp"

#include "../../include/Configuration.hpp"
#include "../../include/GameData/BlockStates/WaterBlockState.hpp"
#include "../../include/Utility.hpp"

#include <mutex>

void ChunkComponent::setBlock(int x, int y, int z, const Block& block) {
    glm::vec3 blockPosition{x, y, z};
    std::lock_guard<std::shared_mutex> lock{*blockMutex};

    // state
    // if needed delete old state and create new state
    switch (block.type) {
        case BlockId::BLOCK_WATER:           
            blockStates.createBlockState<WaterBlockState>(blockPosition);
            break;

        default:
            break;
    }

    blocks[x][y][z] = block.type;
}

void ChunkComponent::setBlock(const glm::vec3& position, const Block& block) {
    std::lock_guard<std::shared_mutex> lock{*blockMutex};

    // state
    // if needed delete old state and create new state
    switch (block.type) {
        case BlockId::BLOCK_WATER:            
            blockStates.createBlockState<WaterBlockState>(position);
            break;

        default:
            break;
    }

    blocks[(int)position.x][(int)position.y][(int)position.z] = block.type;
}

Block ChunkComponent::getBlock(int x, int y, int z) const {
    if (inRange(x, 0, Configuration::CHUNK_SIZE) && inRange(y, 0, Configuration::CHUNK_HEIGHT) && inRange(z, 0, Configuration::CHUNK_SIZE)) {
        BlockId type = blocks[x][y][z];

        return Block{Utility::GetWorldCoords(chunkX, chunkZ, x, y, z), type};
    }
    else {
        return Block{};
    }
}

Block ChunkComponent::getBlock(const glm::vec3& position) const {
    return getBlock(position.x, position.y, position.z);
}

Block ChunkComponent::getBlock(int x, int y, int z) {
    if (Utility::inChunk(x, y, z)) {
        BlockId type = blocks[x][y][z];

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
