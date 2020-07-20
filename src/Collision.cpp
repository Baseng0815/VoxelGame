#include "../include/Collision.h"
#include "../include/Utility.h"

BoxCollision::BoxCollision(glm::vec3 pos, float width, float height, float length) : relativePos(pos), width(width), height(height), length(length) {}

std::vector<glm::vec3> BoxCollision::getAffectedBlocks(glm::vec3 position = glm::vec3()) const {
    std::vector<glm::vec3> positions = std::vector<glm::vec3>();

    for (int x = 0; x <= width; x++)
        for (int y = 0; y <= height; y++)
            for (int z = 0; z <= length; z++) {
                positions.push_back(glm::floor(relativePos + position + glm::vec3(x, y, z)));
            }

    return positions;
}

glm::vec3 BoxCollision::getCenter(glm::vec3 position = glm::vec3())const {
    return relativePos + position + 0.5f * glm::vec3(width, height, length);
}

void BoxCollision::getMinMax(const glm::vec3 position, glm::vec3* min, glm::vec3* max) const {
    glm::vec3 pos1 = relativePos + position;
    glm::vec3 pos2 = relativePos + position + glm::vec3(width, height, length);

    if (pos1.x < pos2.x) {
        min->x = pos1.x;
        max->x = pos2.x;
    }
    else {
        min->x = pos2.x;
        max->x = pos1.x;
    }

    if (pos1.y < pos2.y) {
        min->y = pos1.y;
        max->y = pos2.y;
    }
    else {
        min->y = pos2.y;
        max->y = pos1.y;
    }

    if (pos1.z < pos2.z) {
        min->z = pos1.z;
        max->z = pos2.z;
    }
    else {
        min->z = pos2.z;
        max->z = pos1.z;
    }
}

Ray::Ray(glm::vec3 start, glm::vec3 dir) : start(start), direction(glm::normalize(dir)) {}

std::vector<glm::vec3> Ray::getIntersectionBlocks(float length) const {
    /*std::vector<glm::vec3> blocks = std::vector<glm::vec3>();			

      float dx = length * direction.x;
      float dy = length * direction.y;
      float dz = length * direction.z;	

      return blocks;*/
    std::vector<glm::vec3> blocks = std::vector<glm::vec3>();
    glm::vec3 lastPos = glm::vec3(INT32_MAX, INT32_MAX, INT32_MAX);

    int count = ceil(length);
    for (int i = 0; i < count; i++) {
        glm::vec3 pos = round(start + (float)i * direction);

        if (pos != lastPos) {
            blocks.push_back(pos);
            lastPos = pos;
        }
    }

    return blocks;
}

glm::vec3 Ray::getIntersectionFace(glm::vec3 block) const {
    float fractions[] = {
        // x
        (block.x + 1 - start.x) / direction.x,
        (block.x - start.x) / direction.x,

        // y
        (block.y + 1 - start.y) / direction.y,
        (block.y - start.y) / direction.y,

        // z
        (block.z + 1 - start.z) / direction.z,
        (block.z - start.z) / direction.z
    };

    float minFrac = FLT_MAX;
    for (auto p : fractions) {
        if (p < minFrac) {
            glm::vec3 point = start + p * direction;

            glm::vec3 pb = abs(block - point);

            // check if intersection point is on block face
            bool onFace =
                pb.x >= 0 && pb.x <= 1 &&
                pb.y >= 0 && pb.y <= 1 &&
                pb.z >= 0 && pb.z <= 1;

            if (onFace) {
                minFrac = p;
            }
        }
    }

    glm::vec3 intersectionPoint = start + minFrac * direction;

    // left
    if (intersectionPoint.x == block.x)
        return glm::vec3(-1, 0, 0);
    // right
    else if (intersectionPoint.x == block.x + 1)
        return glm::vec3(1, 0, 0);
    // down
    else if (intersectionPoint.y == block.y)
        return glm::vec3(0, -1, 0);
    // up
    else if (intersectionPoint.y == block.y + 1)
        return glm::vec3(0, 1, 0);
    // front
    else if (intersectionPoint.z == block.z)
        return glm::vec3(0, 0, -1);
    // back
    else if (intersectionPoint.z == block.z + 1)
        return glm::vec3(0, 0, 1);
}

#pragma region Collision

bool Collision::intersects(const BoxCollision* colA, const glm::vec3& posA,
        const BoxCollision* colB, const glm::vec3& posB) {
    glm::vec3 aMin, bMin, aMax, bMax;

    colA->getMinMax(posA, &aMin, &aMax);
    colB->getMinMax(posB, &bMin, &bMax);

    return aMin.x <= bMax.x && aMax.x >= bMin.x &&
        aMin.y <= bMax.y && aMax.y >= bMin.y &&
        aMin.z <= bMax.z && aMax.z >= bMin.z;
}

std::vector<glm::vec3> Collision::getTranslationVectors(const BoxCollision* colA, const glm::vec3& posA,
        const BoxCollision* colB, const glm::vec3& posB) {
    std::vector<glm::vec3> translationVectors = std::vector<glm::vec3>();
    glm::vec3 aMin, bMin, aMax, bMax;

    colA->getMinMax(posA, &aMin, &aMax);
    colB->getMinMax(posB, &bMin, &bMax);

    // x
    translationVectors.push_back(glm::vec3(bMax.x - aMin.x, 0, 0));
    translationVectors.push_back(glm::vec3(bMin.x - aMax.x, 0, 0));

    // y
    translationVectors.push_back(glm::vec3(0, bMax.y - aMin.y, 0));
    translationVectors.push_back(glm::vec3(0, bMin.y - aMax.y, 0));

    // z
    translationVectors.push_back(glm::vec3(0, 0, bMax.z - aMin.z));
    translationVectors.push_back(glm::vec3(0, 0, bMin.z - aMax.z));

    return translationVectors;
}

#pragma endregion
