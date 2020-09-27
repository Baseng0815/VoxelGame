//#include "../include/Utility.hpp"

#include "../include/Components/WorldComponent.hpp"
#include "../include/Components/InventoryComponent.hpp"
#include "../include/Configuration.hpp"

#include <iostream>

namespace Utility {
    long HashFunctionVec2::operator()(const glm::vec2& vec) const {
        return std::hash<float>{}(vec.x) ^ (std::hash<float>{}(vec.y) << 1);
    }

    bool InChunk(const glm::vec3& pos) {
        return pos.x >= 0 && pos.x < Configuration::CHUNK_SIZE && pos.y >= 0 && pos.y < Configuration::CHUNK_HEIGHT &&
            pos.z >= 0 && pos.z < Configuration::CHUNK_SIZE;
    }

    bool InChunk(const int x, const int y, const int z) {
        return x >= 0 && x < Configuration::CHUNK_SIZE&& y >= 0 && y < Configuration::CHUNK_HEIGHT&& z >= 0 && z < Configuration::CHUNK_SIZE;
    }

    std::tuple<glm::vec2, glm::vec3> GetChunkAndLocal(const glm::vec3& worldCoords) {
        glm::vec2 chunkPos {std::floor(worldCoords.x / Configuration::CHUNK_SIZE),
            std::floor(worldCoords.z / Configuration::CHUNK_SIZE)};

        glm::vec3 localCoords {
            worldCoords.x - std::floor(worldCoords.x / Configuration::CHUNK_SIZE) * Configuration::CHUNK_SIZE,
            worldCoords.y,
            worldCoords.z - std::floor(worldCoords.z / Configuration::CHUNK_SIZE) * Configuration::CHUNK_SIZE
        };

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

    glm::vec3 GetBlockCoords(const glm::vec3& coords) {
        glm::vec3 block = glm::vec3();

        for (int i = 0; i < 3; i++) {
            float a = coords[i] - floor(coords[i]);

            block[i] = a <= 0.5 ? floor(coords[i]) : ceil(coords[i]);
        }

        return block;
    }

    glm::vec3 getBlockNormal(glm::vec3 moveDir) {
        float maxAbs = 0;
        glm::vec3 normal{};

        for (int i = 0; i < 3; i++) {
            float component = moveDir[i];

            if (std::fabs(component) > maxAbs) {
                maxAbs = std::fabs(component);
                normal = glm::vec3{};

                normal[i] = component < 0 ? 1 : -1;
            }
        }

        return normal;
    }

    glm::vec3 getFacePosition(const glm::vec3& blockPos, const glm::vec3& faceNormal) {
        glm::vec3 faceOffset = glm::vec3{0.5f * faceNormal.x * faceNormal.x + 0.5f * faceNormal.x,
            0.5f * faceNormal.y * faceNormal.y + 0.5f * faceNormal.y,
            0.5f * faceNormal.z * faceNormal.z + 0.5f * faceNormal.z};

        return blockPos + faceOffset;
    }

    template <typename T> void swap(std::vector<T>& vec, int pos1, int pos2) {
        T tmp = vec[pos2];
        vec[pos2] = vec[pos1];
        vec[pos1] = tmp;
    }

    float radToDeg(float rad) {
        return rad * (180 / PI);
    }    

    int getInventorySlot(const InventoryComponent& inventory, BlockId item) {
        for (int i = 0; i < inventory.slotsCount; i++) {
            if(inventory.slots[i].first == item) {
                if(inventory.slots[i].second < 64) {
                    return i;
                }
            }
            else if(inventory.slots[i].first == BlockId::BLOCK_AIR){
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
