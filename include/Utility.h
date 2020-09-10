#pragma once

#include <glm/glm.hpp>

#define BUFFER_OFFSET(x) ((void*)(x))
#define PI 3.1415926535897932384626433832f

#define randNext(min, max) ((min + (max-min) * (float)rand() / RAND_MAX))
#define sign(x) (((x) < 0) ? (-1) : 1)

#include <vector>
#include <utility>
#include <ostream>
#include <functional>

#include "../include/Block.h"

struct Block;
struct WorldComponent;

namespace Utility {
    class HashFunctionVec2 {
        public:
            long operator()(const glm::vec2 &vec) const;
    };

    bool InChunk(const glm::vec3 &position);

    std::tuple<glm::vec2, glm::vec3> GetChunkAndLocal(const glm::vec3 &worldCoords);
    glm::vec2 GetChunk(const glm::vec3 &worldCoords);
    glm::vec3 GetWorldCoords(const glm::vec2 &chunk, const glm::vec3 &chunkCoords);
    glm::vec3 GetChunkCoords(const glm::vec3 &worldPos);
    glm::vec3 GetBlockCoords(const glm::vec3 &coords);

    template<typename T>
    void swap(std::vector<T> &vec, int pos1, int pos2);

    void FillSphere(glm::vec3 center, float radius, Block*** blocks, BlockType fillType = BlockType::BLOCK_AIR);
}

glm::vec3 getEuler(glm::vec3 v, float a);

glm::vec3 getFacePosition(glm::vec3 blockPos, glm::vec3 faceNormal);

std::ostream &operator<<(std::ostream &os, const glm::vec2 &vec);
std::ostream &operator<<(std::ostream &os, const glm::vec3 &vec);
