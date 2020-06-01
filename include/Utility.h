#pragma once
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
