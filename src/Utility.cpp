#include "../include/Utility.hpp"

#include "../include/Components/InventoryComponent.hpp"
#include "../include/Configuration.hpp"

#include <iostream>

namespace Utility {
    bool InChunk(const glm::vec3& pos) {
        return pos.x >= 0 && pos.x < Configuration::CHUNK_SIZE && pos.y >= 0 && pos.y < Configuration::CHUNK_HEIGHT &&
            pos.z >= 0 && pos.z < Configuration::CHUNK_SIZE;
    }

    bool InChunk(const int x, const int y, const int z) {
        return x >= 0 && x < Configuration::CHUNK_SIZE && y >= 0 && y < Configuration::CHUNK_HEIGHT && z >= 0 && z < Configuration::CHUNK_SIZE;
    }

    std::tuple<glm::vec2, glm::vec3> GetChunkAndLocal(const glm::vec3& worldCoords) {
        glm::vec2 chunkPos{std::floor(worldCoords.x / Configuration::CHUNK_SIZE),
                        std::floor(worldCoords.z / Configuration::CHUNK_SIZE)};

        glm::vec3 localCoords{
            worldCoords.x - std::floor(worldCoords.x / Configuration::CHUNK_SIZE) * Configuration::CHUNK_SIZE,
            worldCoords.y,
            worldCoords.z - std::floor(worldCoords.z / Configuration::CHUNK_SIZE) * Configuration::CHUNK_SIZE};

        return std::make_tuple(chunkPos, localCoords);
    }

    glm::vec2 GetChunk(const glm::vec3& worldCoords) {
        return glm::vec2{std::floor(worldCoords.x / Configuration::CHUNK_SIZE),
                        std::floor(worldCoords.z / Configuration::CHUNK_SIZE)};
    }

    glm::vec3 GetWorldCoords(const glm::vec2& chunk, const glm::vec3& chunkCoords) {
        glm::mat2x3 chunkTransform = glm::mat2x3(0.0f);
        chunkTransform[0][0] = Configuration::CHUNK_SIZE;
        chunkTransform[1][2] = Configuration::CHUNK_SIZE;

        return chunkTransform * chunk + chunkCoords;
    }

    glm::vec3 GetChunkCoords(const glm::vec3& worldCoords) {
        int cx = worldCoords.x - std::floor(worldCoords.x / Configuration::CHUNK_SIZE) * Configuration::CHUNK_SIZE;
        int cz = worldCoords.z - std::floor(worldCoords.z / Configuration::CHUNK_SIZE) * Configuration::CHUNK_SIZE;

        return glm::vec3{cx, worldCoords.y, cz};
    }

    std::vector<glm::vec3> getNeighborBlocks(const glm::vec3& blockPos, bool moore) {
        std::vector<glm::vec3> positions;

        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                for (int z = -1; y <= 1; z++) {
                    if(y < 256 && y >= 0) {
                        glm::vec3 offset = glm::vec3{x, y, z};
                        float length = glm::length(offset);
                        if(length == 0) {
                            continue;
                        }
                        else if(length > 1 && moore) {
                            positions.emplace_back(blockPos + offset);
                            continue;
                        }

                        positions.emplace_back(blockPos + offset);
                    }
                }
            }
        }

        return positions;
    }

    int getInventorySlot(const InventoryComponent& inventory, BlockId item) {
        for (int i = 0; i < inventory.slotsCount; i++) {
            if (inventory.slots[i].first == item) {
                if (inventory.slots[i].second < 64) {
                    return i;
                }
            }
            else if (inventory.slots[i].first == BlockId::BLOCK_AIR) {
                return i;
            }
        }

        return -1;
    }
} // namespace Utility

std::ostream& operator<<(std::ostream& os, const glm::vec2& vec) {
    os << "x: " << vec.x << " ";
    os << "y: " << vec.y;
    return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& vec) {
    os << "x: " << vec.x << " ";
    os << "y: " << vec.y << " ";
    os << "z: " << vec.z;
    return os;
}
