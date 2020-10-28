#pragma once

#include <glm/glm.hpp>

#define BUFFER_OFFSET(x) ((void*)(x))
#define PI 3.1415926535897932384626433832f

#define randNext(min, max) ((min + (max - min) * (float)rand() / RAND_MAX))
#define sign(x) (((x) < 0) ? (-1) : 1)

#include <functional>
#include <ostream>
#include <utility>
#include <vector>

#include "GameData/Block.hpp"

struct WorldComponent;
struct InventoryComponent;

namespace Utility {
    bool InChunk(const glm::vec3& position);
    bool InChunk(const int x, const int y, const int z);

    std::tuple<glm::vec2, glm::vec3> GetChunkAndLocal(const glm::vec3& worldCoords);
    glm::vec2 GetChunk(const glm::vec3& worldCoords);
    glm::vec3 GetWorldCoords(const glm::vec2& chunk, const glm::vec3& chunkCoords);
    glm::vec3 GetChunkCoords(const glm::vec3& worldPos);
    glm::vec3 GetBlockCoords(const glm::vec3& coords);

    glm::vec3 getFacePosition(const glm::vec3& blockPos, const glm::vec3& faceNormal);

    template <typename T>
    void swap(std::vector<T>& vec, int pos1, int pos2);

    float radToDeg(float rad);

    int getInventorySlot(const InventoryComponent& inventory, BlockId item);
    } // namespace Utility

    std::ostream& operator<<(std::ostream& os, const glm::vec2& vec);
    std::ostream& operator<<(std::ostream& os, const glm::vec3& vec);
