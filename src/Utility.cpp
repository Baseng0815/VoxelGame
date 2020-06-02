#include "../include/Utility.h"
#include "../include/Configuration.h"

glm::vec3* GetCavePoints(glm::vec2 chunk, int count, int seed) {
    glm::vec3* points = new glm::vec3[count];

    for(int i = 0; i < count; i++) {
        int x = ((rand() + seed) & 0xFFFFFFFF) % CHUNK_SIZE;
        int y = ((rand() + seed) & 0xFFFFFFFF) % CHUNK_HEIGHT;
        int z = ((rand() + seed) & 0xFFFFFFFF) % CHUNK_SIZE;

        points[i] = glm::vec3(x, y, z);
    }

    return points;
}

glm::vec3* GetBezierPoints(glm::vec3* points, int count, int* resultCount) {
    float t = 0; 
    float step = 0.1;
    glm::vec3* bezierPoints = new glm::vec3[1 / step];

    for(; t <= 1; t += step) {
        bezierPoints[(int)(1 / t)] = GetBezierPoint(points, count, t);
    }

    *resultCount = 1 / step;

    return bezierPoints;
}

glm::vec3 GetBezierPoint(glm::vec3* points, int count, float t) {
    if(count == 1)
        return points[0];

    glm::vec3* newPoints = new glm::vec3[count - 1];

    for (int i = 0; i < count - 1; i++){
        glm::vec3 dP = points[i + 1] - points[i];

        newPoints[i] = points[i] + t * dP;
    }

    glm::vec3 point = GetBezierPoint(newPoints, count - 1, t);
    delete[] points;

    return point;
}

#include "../include/Block.h"

void FillSphere(glm::vec3 center, float radius, Block*** blocks, BlockType fillType) {
    
}