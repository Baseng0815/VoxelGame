#pragma once

#include "Block.h"

#include <glm/glm.hpp>

#define BUFFER_OFFSET(x) ((void*)(x))
#define PI 3.1415926535897932384626433832f

#define randNext(min, max) ((min + (max-min) * (float)rand() / RAND_MAX))
#define min(x, y) (((x) < (y)) ? (x) : (y))

#include <utility>
#include <functional>

class HashFunction {
    public:
        inline long operator()(const glm::vec2& vec) const {
            return std::hash<float>{}(vec.x) ^ (std::hash<float>{}(vec.y) << 1);
        }
};

glm::vec2 GetChunk(glm::vec3 worldCoords, glm::vec3& localCoords);

glm::vec3 GetWorldCoords(glm::vec2 chunk, glm::vec3 chunkCoords);

glm::vec3 GetChunkCoords(glm::vec3 worldPos);

template<typename T>
void swap(std::vector<T>* vec, int pos1, int pos2);

bool InChunk(glm::vec3 position);

<<<<<<< HEAD
struct Block;

void FillSphere(glm::vec3 center, float radius, Block*** blocks, char fillType = 0);
=======
void FillSphere(glm::vec3 center, float radius, Block*** blocks, BlockType fillType = BlockType::BLOCK_AIR);
>>>>>>> 46c21a7c554023057da23128351cce523f4a8bed
