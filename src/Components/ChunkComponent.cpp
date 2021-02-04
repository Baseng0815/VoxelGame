#include "../../include/Components/ChunkComponent.hpp"

#include "../../include/Configuration.hpp"
#include "../../include/GameData/BlockStates/WaterBlockState.hpp"
#include "../../include/Utility/Utility.hpp"

#include <mutex>

void ChunkComponent::setBlock(const Block& block) {
    glm::ivec3 localPos = Utility::getLocalCoords(block.position);
    std::lock_guard<std::shared_mutex> lock{*blockMutex};

    // state
    // if needed delete old state and create new state
    switch (block.type) {
    case BlockId::BLOCK_WATER:
        blockStates.createBlockState<WaterBlockState>(localPos);
        break;

    default:
        break;
    }

    blocks[localPos.x][localPos.y][localPos.z] = block.type;
}

Block ChunkComponent::getBlock(int x, int y, int z) const {
    if (Utility::inChunk(x, y, z)) {
        BlockId type = blocks[x][y][z];

        return Block{Utility::getWorldCoords(chunkX, chunkZ, x, y, z), type};
    }
    else {
        return Block{};
    }
}

Block ChunkComponent::getBlock(const glm::ivec3& position) const {
    return getBlock(position.x, position.y, position.z);
}

Block ChunkComponent::getBlock(int x, int y, int z) {
    if (Utility::inChunk(x, y, z)) {
        BlockId type = blocks[x][y][z];

        Block block;
        switch (type) {
        case BlockId::BLOCK_WATER: {
            WaterBlockState* state = blockStates.getState<WaterBlockState>(glm::vec3{x, y, z});
            block = Block{Utility::getWorldCoords(chunkX, chunkZ, x, y, z), type, state};
        } break;
        default:
            block = Block{Utility::getWorldCoords(chunkX, chunkZ, x, y, z), type, nullptr};
            break;
        }

        return block;
    }
    else {
        return Block{};
    }
}

Block ChunkComponent::getBlock(const glm::ivec3& position) {
    return getBlock(position.x, position.y, position.z);
}
