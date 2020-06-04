#include "../include/Utility.h"
#include "../include/Configuration.h"

glm::vec2 GetChunk(glm::vec3 worldCoords, glm::vec3& localCoords) {
    glm::vec2 chunkPos;

    chunkPos.x = (int)worldCoords.x / CHUNK_SIZE;
    chunkPos.y = (int)worldCoords.z / CHUNK_SIZE;
    int cx = (int)worldCoords.x % CHUNK_SIZE;
    int cz = (int)worldCoords.z % CHUNK_SIZE;

    if(cx < 0)
        cx = CHUNK_SIZE - abs(cx);
    if(cz < 0)
        cz = CHUNK_SIZE - abs(cz);

<<<<<<< HEAD
    if(worldCoords.x < 0 && cx != 0) {
        chunkPos.x -= 1;
=======
glm::vec3* GetBezierPoints(glm::vec3* points, int count, int* resultCount) {
    float t = 0;
    float step = 0.1;
    // TODO replace dynamic arrays with std::vector
    glm::vec3* bezierPoints = new glm::vec3[(int)(1 / step)];

    for(; t <= 1; t += step) {
        bezierPoints[(int)(1 / t)] = GetBezierPoint(points, count, t);
>>>>>>> 46c21a7c554023057da23128351cce523f4a8bed
    }

    if(worldCoords.z < 0 && cz != 0) {
        chunkPos.y -= 1;
    }

    localCoords = glm::vec3(cx, worldCoords.y, cz);
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

<<<<<<< HEAD
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
=======
void FillSphere(glm::vec3 center, float radius, Block*** blocks, BlockType fillType) {

}
>>>>>>> 46c21a7c554023057da23128351cce523f4a8bed
