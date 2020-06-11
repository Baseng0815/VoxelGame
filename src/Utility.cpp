#include "../include/Utility.h"
#include "../include/Configuration.h"

#include "../include/Components/WorldComponent.h"
#include <iostream>

#include <iostream>

glm::vec2 GetChunk(glm::vec3 worldCoords, glm::vec3& localCoords) {
    glm::vec2 chunkPos;

    /*chunkPos.x = (int)worldCoords.x / CHUNK_SIZE;
    chunkPos.y = (int)worldCoords.z / CHUNK_SIZE;
    int cx = (int)worldCoords.x % CHUNK_SIZE;
    int cz = (int)worldCoords.z % CHUNK_SIZE;

    if(cx < 0)
        cx = CHUNK_SIZE - abs(cx);
    if(cz < 0)
        cz = CHUNK_SIZE - abs(cz);

    if(worldCoords.x < 0 && cx != 0) {
        chunkPos.x -= 1;
    }

    if(worldCoords.z < 0 && cz != 0) {
        chunkPos.y -= 1;
    }

    localCoords = glm::vec3(cx, worldCoords.y, cz);*/
    chunkPos.x = floor((worldCoords.x + 0.5) / CHUNK_SIZE);
    chunkPos.y = floor((worldCoords.z + 0.5) / CHUNK_SIZE);

    localCoords.x = CHUNK_SIZE * (worldCoords.x / CHUNK_SIZE - floor(worldCoords.x / CHUNK_SIZE));
    localCoords.y = worldCoords.y;
    localCoords.z = CHUNK_SIZE * (worldCoords.z / CHUNK_SIZE - floor(worldCoords.z / CHUNK_SIZE));

    return chunkPos;
}

glm::vec3 GetWorldCoords(glm::vec2 chunk, glm::vec3 chunkCoords) {    
    glm::mat2x3 chunkTransform = glm::mat2x3(0.0f);     
    chunkTransform[0][0] = CHUNK_SIZE;
    chunkTransform[1][2] = CHUNK_SIZE;

    return chunkTransform * chunk + chunkCoords;
}

glm::vec3 GetChunkCoords(glm::vec3 worldCoords) {
    int cx = (int)worldCoords.x % CHUNK_SIZE;
    int cz = (int)worldCoords.z % CHUNK_SIZE;

    if (cx < 0)
        cx = CHUNK_SIZE - abs(cx);
    if (cz < 0)
        cz = CHUNK_SIZE - abs(cz);

    return glm::vec3(cx, worldCoords.y, cz);
}

template<typename T>
void swap(std::vector<T>* vec, int pos1, int pos2) {
    T tmp = vec[pos2];
    vec[pos2] = vec[pos1];
    vec[pos1] = tmp;
}

bool InChunk(glm::vec3 pos){
    return pos.x >= 0 && pos.x < CHUNK_SIZE && pos.y >= 0 && pos.y < CHUNK_HEIGHT && pos.z >= 0 && pos.z < CHUNK_SIZE;
}

#include "../include/Block.h"

void FillSphere(glm::vec3 center, float radius, Block*** blocks, char fillType) {
    float x, y, z;
    float radiusSquare = radius * radius;

    for(x = -radius; x <= radius; x++) {
        float zMax = sqrt(radiusSquare - x * x);
        for(z = -zMax; z <= zMax; z++) {
            float yMax = sqrt(radiusSquare - (x * x + z * z));
            for(y = -yMax; y <= yMax; y++) {
                glm::vec3 position = center + glm::vec3(x, y, z);
                if(InChunk(position)) {
                    blocks[(int)position.x][(int)position.y][(int)position.z] = Block((BlockType)fillType);
                }
            }
        }
    }
}

std::ostream& operator<<(std::ostream& os, const glm::vec2 &vec) {
    os << "x: " << vec.x << " ";
    os << "y: " << vec.y;
    return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec3 &vec) {
    os << "x: " << vec.x << " ";
    os << "y: " << vec.y << " ";
    os << "z: " << vec.z;
    return os;
}
