#include "../include/Utility.hpp"

#include "../include/Components/ChunkComponent.hpp"
#include "../include/Components/InventoryComponent.hpp"
#include "../include/Configuration.hpp"
#include "../include/World.hpp"

#include "../include/Math/Ray.hpp"

#include <iostream>

namespace Utility {

    bool inChunk(const glm::ivec3& pos) {
        return pos.x >= 0 && pos.x < Configuration::CHUNK_SIZE && pos.y >= 0 && pos.y < Configuration::CHUNK_HEIGHT &&
               pos.z >= 0 && pos.z < Configuration::CHUNK_SIZE;
    }

    bool inChunk(const int x, const int y, const int z) {
        return x >= 0 && x < Configuration::CHUNK_SIZE && y >= 0 && y < Configuration::CHUNK_HEIGHT && z >= 0 && z < Configuration::CHUNK_SIZE;
    }

    bool onBlockFace(glm::ivec3 block, glm::vec3 position) {
        glm::vec3 relativePos = position - (glm::vec3)block;

        // clang-format off
        return relativePos.x >= 0 && relativePos.x <= 1
            && relativePos.y >= 0 && relativePos.y <= 1
            && relativePos.z >= 0 && relativePos.z <= 1;
        // clang-format on
    }

    std::tuple<glm::vec2, glm::ivec3> GetChunkAndLocal(const glm::ivec3& worldCoords) {
        glm::vec2 chunkPos{std::floor(worldCoords.x / (float)Configuration::CHUNK_SIZE),
                           std::floor(worldCoords.z / (float)Configuration::CHUNK_SIZE)};

        glm::vec3 localCoords{
            worldCoords.x - std::floor(worldCoords.x / (float)Configuration::CHUNK_SIZE) * Configuration::CHUNK_SIZE,
            worldCoords.y,
            worldCoords.z - std::floor(worldCoords.z / (float)Configuration::CHUNK_SIZE) * Configuration::CHUNK_SIZE};

        return std::make_tuple(chunkPos, localCoords);
    }

    glm::ivec2 getChunk(const glm::ivec3& worldCoords) {
        return glm::ivec2{(int)std::floor(worldCoords.x / (float)Configuration::CHUNK_SIZE),
                          (int)std::floor(worldCoords.z / (float)Configuration::CHUNK_SIZE)};
    }

    glm::ivec3 getWorldCoords(const glm::ivec2& chunk, const glm::ivec3& chunkCoords) {
        glm::ivec3 chunkOffset = glm::ivec3{chunk.x * Configuration::CHUNK_SIZE, 0, chunk.y * Configuration::CHUNK_SIZE};
        return chunkOffset + chunkCoords;
    }

    glm::ivec3 getWorldCoords(const int chunkX, const int chunkZ, const int x, const int y, const int z) {
        int cx = chunkX * Configuration::CHUNK_SIZE + x;
        int cy = y;
        int cz = chunkZ * Configuration::CHUNK_SIZE + z;

        return glm::ivec3{cx, cy, cz};
    }

    glm::ivec3 getLocalCoords(const glm::ivec3& worldCoords) {
        int cx = worldCoords.x - std::floor(worldCoords.x / (float)Configuration::CHUNK_SIZE) * Configuration::CHUNK_SIZE;
        int cz = worldCoords.z - std::floor(worldCoords.z / (float)Configuration::CHUNK_SIZE) * Configuration::CHUNK_SIZE;

        return glm::ivec3{cx, worldCoords.y, cz};
    }

    std::vector<glm::ivec3> getNeighborBlocks(const glm::ivec3& blockPos, bool moore) {
        std::vector<glm::ivec3> positions;
        positions.emplace_back(blockPos.x - 1, blockPos.y, blockPos.z);
        positions.emplace_back(blockPos.x + 1, blockPos.y, blockPos.z);
        positions.emplace_back(blockPos.x, blockPos.y - 1, blockPos.z);
        positions.emplace_back(blockPos.x, blockPos.y + 1, blockPos.z);
        positions.emplace_back(blockPos.x, blockPos.y, blockPos.z - 1);
        positions.emplace_back(blockPos.x, blockPos.y, blockPos.z + 1);

        if (moore) {
            positions.emplace_back(blockPos.x - 1, blockPos.y - 1, blockPos.z - 1);
            positions.emplace_back(blockPos.x - 1, blockPos.y - 1, blockPos.z + 1);
            positions.emplace_back(blockPos.x - 1, blockPos.y + 1, blockPos.z - 1);
            positions.emplace_back(blockPos.x - 1, blockPos.y + 1, blockPos.z + 1);
            positions.emplace_back(blockPos.x + 1, blockPos.y - 1, blockPos.z - 1);
            positions.emplace_back(blockPos.x + 1, blockPos.y - 1, blockPos.z + 1);
            positions.emplace_back(blockPos.x + 1, blockPos.y + 1, blockPos.z - 1);
            positions.emplace_back(blockPos.x + 1, blockPos.y + 1, blockPos.z + 1);
        }

        return positions;
    }

    glm::vec3 getBlockIntersectionFaceNormal(const glm::vec3& blockPos, const Math::Ray& lookDirection) {
        float minLambda = FLT_MAX;
        glm::vec3 result = glm::vec3{0};

        glm::vec3 origin = lookDirection.origin;
        glm::vec3 direction = lookDirection.getDirection();

        float lambda = 0;

        // positive x
        lambda = (blockPos.x + 1 - origin.x) / direction.x;
        if (lambda < minLambda) {
            minLambda = lambda;
            result = glm::vec3{1, 0, 0};
        }

        // negative x
        lambda = (blockPos.x - origin.x) / direction.x;
        if (lambda < minLambda) {
            minLambda = lambda;
            result = glm::vec3{-1, 0, 0};
        }

        // positive y
        lambda = (blockPos.y + 1 - origin.y) / direction.y;
        if (lambda < minLambda) {
            minLambda = lambda;
            result = glm::vec3{0, 1, 0};
        }

        // negative y
        lambda = (blockPos.y - origin.y) / direction.y;
        if (lambda < minLambda) {
            minLambda = lambda;
            result = glm::vec3{0, -1, 0};
        }

        // positive z
        lambda = (blockPos.z + 1 - origin.z) / direction.z;
        if (lambda < minLambda) {
            minLambda = lambda;
            result = glm::vec3{0, 1, 0};
        }

        lambda = (blockPos.z - origin.z) / direction.z;
        if (lambda < minLambda) {
            minLambda = lambda;
            result = glm::vec3{0, -1, 0};
        }

        return result;
    }

    void chunk_execute(entt::registry& registry, ChunkComponent& inChunk, const int& cx, const int& cy, const int& cz, std::function<void(ChunkComponent&, const int&, const int&, const int&)> action) {
        if (cy < 0 || cy >= Configuration::CHUNK_HEIGHT) {
            return;
        }

        int x = cx;
        int y = cy;
        int z = cz;
        int chunkXOff = 0;
        int chunkZOff = 0;

        if (x < 0 || x >= Configuration::CHUNK_SIZE) {
            chunkXOff = floor(x / (float)Configuration::CHUNK_SIZE);
            x = x - floor(x / (float)Configuration::CHUNK_SIZE) * Configuration::CHUNK_SIZE;
        }

        if (z < 0 || z >= Configuration::CHUNK_SIZE) {
            chunkZOff = floor(z / (float)Configuration::CHUNK_SIZE);
            z = z - floor(z / (float)Configuration::CHUNK_SIZE) * Configuration::CHUNK_SIZE;
        }

        if (chunkXOff != 0 || chunkZOff != 0) {
            ChunkComponent& other = World::getChunk(registry, glm::vec2{inChunk.chunkX + chunkXOff, inChunk.chunkZ + chunkZOff});
            action(other, x, y, z);
        }
        else {
            action(inChunk, x, y, z);
        }
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
