#pragma once

#include <glm/glm.hpp>

// TODO put this all into functions and constexpr
#define BUFFER_OFFSET(x) ((void*)(x))
// #define PI 3.1415926535897932384626433832f

#define randNext(min, max) ((min + (max - min) * (float)rand() / RAND_MAX))
// #define sign(x) (((x) < 0) ? (-1) : 1)
#define inRange(x, min, max) (x >= min && x < max)

#include <entt/entt.hpp>
#include <functional>
#include <ostream>
#include <utility>
#include <vector>

#include "../GameData/Block.hpp"
#include "../Math/Ray.hpp"

struct WorldComponent;
struct InventoryComponent;
struct ChunkComponent;

namespace Utility {
    constexpr float PI = 3.141592741f;

    bool onBlockFace(glm::ivec3 block, glm::vec3 position);

    bool inChunk(const glm::ivec3& position);
    bool inChunk(const int x, const int y, const int z);

    bool inChunk(const glm::ivec3& position, const glm::ivec2& chunk);
    bool inChunk(const int x, const int y, const int z, const int chunkX, const int chunkZ);

    std::tuple<glm::vec2, glm::ivec3> GetChunkAndLocal(const glm::ivec3& worldCoords);
    glm::ivec2 getChunk(const glm::ivec3& worldCoords);
    glm::ivec3 getWorldCoords(const glm::ivec2& chunk, const glm::ivec3& chunkCoords);
    glm::ivec3 getWorldCoords(const int chunkX, const int chunkZ, const int x, const int y, const int z);
    glm::ivec3 getLocalCoords(const glm::ivec3& worldPos);
    glm::ivec3 GetBlockCoords(const glm::ivec3& coords);

    glm::vec3 getBlockIntersectionFaceNormal(const glm::vec3& blockPos, const Math::Ray& lookRay);

    std::vector<glm::ivec3> getNeighborBlocks(const glm::vec3& blockPos, bool moore = false);

    /*Executes an action at the given chunk position with bounds checking */
    void chunk_execute(entt::registry& registry, ChunkComponent& chunk, const int& cx, const int& cy, const int& cz, std::function<void(ChunkComponent&, const int&, const int&, const int&)> action);

    int getInventorySlot(const InventoryComponent& inventory, BlockId item);
} // namespace Utility

std::ostream& operator<<(std::ostream& os, const glm::vec2& vec);
std::ostream& operator<<(std::ostream& os, const glm::vec3& vec);
