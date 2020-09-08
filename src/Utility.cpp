#include "../include/Utility.h"
#include "../include/Configuration.h"

#include "../include/Components/WorldComponent.h"
#include <iostream>

#include <iostream>

glm::vec2 GetChunk(glm::vec3 worldCoords, glm::vec3& localCoords) {
    glm::vec2 chunkPos;
    chunkPos.x = floor(worldCoords.x / Configuration::CHUNK_SIZE);
    chunkPos.y = floor(worldCoords.z / Configuration::CHUNK_SIZE);

    localCoords.x = worldCoords.x - floor(worldCoords.x / Configuration::CHUNK_SIZE) * Configuration::CHUNK_SIZE;
    localCoords.y = worldCoords.y;
    localCoords.z = worldCoords.z - floor(worldCoords.z / Configuration::CHUNK_SIZE) * Configuration::CHUNK_SIZE;

    return chunkPos;
}

glm::vec3 GetWorldCoords(glm::vec2 chunk, glm::vec3 chunkCoords) {
    glm::mat2x3 chunkTransform = glm::mat2x3(0.0f);
    chunkTransform[0][0] = Configuration::CHUNK_SIZE;
    chunkTransform[1][2] = Configuration::CHUNK_SIZE;

    return chunkTransform * chunk + chunkCoords;
}

glm::vec3 GetChunkCoords(glm::vec3 worldCoords) {
    int cx = worldCoords.x - floor(worldCoords.x / Configuration::CHUNK_SIZE) * Configuration::CHUNK_SIZE;
    int cz = worldCoords.z - floor(worldCoords.z / Configuration::CHUNK_SIZE) * Configuration::CHUNK_SIZE;

    return glm::vec3(cx, worldCoords.y, cz);
}

glm::vec3 getBlockCoords(glm::vec3 coords) {
    glm::vec3 block = glm::vec3();

    for (int i = 0; i < 3; i++) {
        float a = coords[i] - floor(coords[i]);

        block[i] = a <= 0.5 ? floor(coords[i]) : ceil(coords[i]);
    }

    return block;
}

glm::vec3 getBlockNormal(glm::vec3 moveDir) {
    float maxAbs = 0;
    glm::vec3 normal = glm::vec3();

    for (int i = 0; i < 3; i++) {
        float component = moveDir[i];

        if (abs(component) > maxAbs) {
            maxAbs = abs(component);
            normal = glm::vec3();

            normal[i] = component < 0 ? 1 : -1;
        }
    }

    return normal;
}

template <typename T> void swap(std::vector<T>* vec, int pos1, int pos2) {
    T tmp = vec[pos2];
    vec[pos2] = vec[pos1];
    vec[pos1] = tmp;
}

bool InChunk(glm::vec3 pos) {
    return pos.x >= 0 && pos.x < Configuration::CHUNK_SIZE && pos.y >= 0 && pos.y < Configuration::CHUNK_HEIGHT &&
           pos.z >= 0 && pos.z < Configuration::CHUNK_SIZE;
}

#include "../include/Block.h"

void FillSphere(glm::vec3 center, float radius, Block*** blocks, char fillType) {
    float x, y, z;
    float radiusSquare = radius * radius;

    for (x = -radius; x <= radius; x++) {
        float zMax = sqrt(radiusSquare - x * x);
        for (z = -zMax; z <= zMax; z++) {
            float yMax = sqrt(radiusSquare - (x * x + z * z));
            for (y = -yMax; y <= yMax; y++) {
                glm::vec3 position = center + glm::vec3(x, y, z);
                if (InChunk(position)) {
                    blocks[(int)position.x][(int)position.y][(int)position.z] = Block((BlockType)fillType);
                }
            }
        }
    }
}

glm::vec3 getEuler(glm::vec3 v, float a) {
    float sinA = sin(a);
    float cosA = cos(a);

    float alpha = atan2(v.z * v.x * (1 - cosA) + v.y * sinA, -v.y * v.z * (1 - cosA) + v.x * sinA);
    float beta = acos(v.z * v.z * (1 - cosA) * cosA);
    float gamma = atan2(v.z * v.x * (1 - cosA) - v.y * sinA, v.z * v.y * (1 - cosA) - v.x * sinA);

    return glm::vec3(alpha, beta, gamma);
}

glm::vec3 getFacePosition(glm::vec3 blockPos, glm::vec3 faceNormal) {
    glm::vec3 faceOffset = glm::vec3{
        0.5f * faceNormal.x * faceNormal.x + 0.5f * faceNormal.x,
        0.5f * faceNormal.y * faceNormal.y + 0.5f * faceNormal.y,
        0.5f * faceNormal.z * faceNormal.z + 0.5f * faceNormal.z
    };

    return blockPos + faceOffset;
}

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
