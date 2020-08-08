#pragma once

#include "Block.h"

#include <glm/glm.hpp>

#define BUFFER_OFFSET(x) ((void*)(x))
#define PI 3.1415926535897932384626433832f

#define randNext(min, max) ((min + (max-min) * (float)rand() / RAND_MAX))
#define sign(x) (((x) < 0) ? (-1) : 1)

#include <vector>
#include <utility>
#include <functional>

struct WorldComponent;

class HashFunction {
    public:
        inline long operator()(const glm::vec2& vec) const {
            return std::hash<float>{}(vec.x) ^ (std::hash<float>{}(vec.y) << 1);
        }
};

glm::vec2 GetChunk(glm::vec3 worldCoords, glm::vec3& localCoords);

glm::vec3 GetWorldCoords(glm::vec2 chunk, glm::vec3 chunkCoords);

glm::vec3 GetChunkCoords(glm::vec3 worldPos);

glm::vec3 getBlockCoords(glm::vec3 coords);

template<typename T>
void swap(std::vector<T>* vec, int pos1, int pos2);

bool InChunk(glm::vec3 position);

struct Block;

void FillSphere(glm::vec3 center, float radius, Block*** blocks, char fillType = 0);

std::ostream &operator<<(std::ostream &os, const glm::vec2 &vec);
std::ostream &operator<<(std::ostream &os, const glm::vec3 &vec);
