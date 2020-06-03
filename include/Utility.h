#pragma once

#include "Block.h"

#include <glm/glm.hpp>

#define BUFFER_OFFSET(x) ((void*)(x))
#define PI 3.1415926535897932384626433832

#define randNext(min, max) ((min + (max-min) * (float)rand() / RAND_MAX))

#include <utility>
#include <functional>

class HashFunction {
    public:
        inline long operator()(const glm::ivec2& vec) const {
            return std::hash<float>{}(vec.x) ^ (std::hash<float>{}(vec.y) << 1);
        }
};

glm::vec3* GetCavePoints(glm::vec2 chunk, int count, int seed = 0);

glm::vec3* GetBezierPoints(glm::vec3* points, int count, int* resultCount);

glm::vec3 GetBezierPoint(glm::vec3* points, int count, float t);

void FillSphere(glm::vec3 center, float radius, Block*** blocks, BlockType fillType = BlockType::BLOCK_AIR);
