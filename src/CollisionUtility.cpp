#include "../include/CollisionUtility.h"
#include "../include/Block.h"
#include "../include/Components/ChunkComponent.h"
#include "../include/Configuration.h"
#include "../include/Cuboid.h"

std::vector<Cuboid> CollisionUtility::getChunkCollisions(const ChunkComponent& chunk) {
    std::vector<Cuboid> collisions = std::vector<Cuboid>();
    for (int i = 0; i < Configuration::CHUNK_HEIGHT; i += Configuration::CHUNK_SIZE) {
        auto col = getCollisions(chunk.blocks, glm::ivec3(0, i, 0), Configuration::CHUNK_SIZE);

        // merge collisions
        while (!col.empty()) {
            Cuboid c1 = col.front();
            auto c1Corners = c1.getCornerPoints();

            bool merged = false;
            for (int i = 1; i < col.size(); i++) {
                Cuboid c2 = col[i];
                auto c2Corners = c2.getCornerPoints();

                std::vector<glm::vec3> commonCorners = std::vector<glm::vec3>();
                for (int j = 0; j < 8; j++) {
                    for (int k = 0; k < 8; k++) {
                        if (c1Corners[j] == c2Corners[k]) {
                            commonCorners.push_back(c1Corners[k]);
                        }
                    }
                }

                // cuboids can merged if they have 4 common corners
                if (commonCorners.size() == 4) {
                    collisions.push_back(Cuboid::merge(c1, c2));
                    merged = true;

                    col.erase(col.begin() + i);
                    break;
                }
            }

            if (!merged) {
                collisions.push_back(c1);
            }

            col.erase(col.begin());
        }
    }

    return collisions;
}

std::vector<Cuboid> CollisionUtility::getCollisions(const Block*** blocks, glm::ivec3 start, int size) {
    std::vector<Cuboid> collisions = std::vector<Cuboid>();
    if (size == 1) {
        // return a collision if block is solid
        if (blocks[start.x][start.y][start.z].isSolid()) {
            collisions.push_back(Cuboid(start, 1, 1, 1));
        }
    }
    else {
        // divide voxel into 8 parts and get collisions
        std::vector<Cuboid> unmergedCollisions = std::vector<Cuboid>();
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    glm::ivec3 newStart = start + glm::ivec3(i * size / 2, j * size / 2, k * size / 2);
                    auto subCollisions = getCollisions(blocks, newStart, size / 2);

                    for (Cuboid collision : subCollisions) {
                        unmergedCollisions.push_back(collision);
                    }
                }
            }
        }

        // merge collisions
        while (!unmergedCollisions.empty()) {
            Cuboid c1 = unmergedCollisions.front();
            auto c1Corners = c1.getCornerPoints();

            bool merged = false;
            for (int i = 1; i < unmergedCollisions.size(); i++) {
                Cuboid c2 = unmergedCollisions[i];
                auto c2Corners = c2.getCornerPoints();

                std::vector<glm::vec3> commonCorners = std::vector<glm::vec3>();
                for (int j = 0; j < 8; j++) {
                    for (int k = 0; k < 8; k++) {
                        if (c1Corners[j] == c2Corners[k]) {
                            commonCorners.push_back(c1Corners[k]);
                        }
                    }
                }

                // cuboids can merged if they have 4 common corners
                if (commonCorners.size() == 4) {
                    collisions.push_back(Cuboid::merge(c1, c2));
                    merged = true;

                    unmergedCollisions.erase(unmergedCollisions.begin() + i);
                    break;
                }
            }

            if (!merged) {
                collisions.push_back(c1);
            }

            unmergedCollisions.erase(unmergedCollisions.begin());
        }
    }

    return collisions;
}