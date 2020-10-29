#include "../../include/GameData/BlockGeometry.hpp"

#include "../../include/GameData/BlockStates/WaterBlockState.hpp"

const GeometryData BlockGeometry::generationDataBlock = {
    //                    x  y  z  nx  ny nz ix iy
    std::array<float, 8>{0, 1, 0, -1, 0, 0, 4, 0},
    std::array<float, 8>{0, 0, 1, -1, 0, 0, 4, 1},
    std::array<float, 8>{0, 1, 1, -1, 0, 0, 4, 2},
    std::array<float, 8>{0, 0, 0, -1, 0, 0, 4, 3},
    std::array<float, 8>{1, 1, 1, 1, 0, 0, 2, 0},
    std::array<float, 8>{1, 0, 0, 1, 0, 0, 2, 1},
    std::array<float, 8>{1, 1, 0, 1, 0, 0, 2, 2},
    std::array<float, 8>{1, 0, 1, 1, 0, 0, 2, 3},
    std::array<float, 8>{0, 0, 1, 0, -1, 0, 5, 0},
    std::array<float, 8>{1, 0, 0, 0, -1, 0, 5, 1},
    std::array<float, 8>{1, 0, 1, 0, -1, 0, 5, 2},
    std::array<float, 8>{0, 0, 0, 0, -1, 0, 5, 3},
    std::array<float, 8>{0, 1, 0, 0, 1, 0, 0, 0},
    std::array<float, 8>{1, 1, 1, 0, 1, 0, 0, 1},
    std::array<float, 8>{1, 1, 0, 0, 1, 0, 0, 2},
    std::array<float, 8>{0, 1, 1, 0, 1, 0, 0, 3},
    std::array<float, 8>{1, 1, 0, 0, 0, -1, 3, 0},
    std::array<float, 8>{0, 0, 0, 0, 0, -1, 3, 1},
    std::array<float, 8>{0, 1, 0, 0, 0, -1, 3, 2},
    std::array<float, 8>{1, 0, 0, 0, 0, -1, 3, 3},
    std::array<float, 8>{0, 1, 1, 0, 0, 1, 1, 0},
    std::array<float, 8>{1, 0, 1, 0, 0, 1, 1, 1},
    std::array<float, 8>{1, 1, 1, 0, 0, 1, 1, 2},
    std::array<float, 8>{0, 0, 1, 0, 0, 1, 1, 3}};

const GeometryData BlockGeometry::generationDataPlane = {
    //                    x  y  z  nx  ny nz fi
    std::array<float, 8>{0, 0, 0, 1, 0, -1, 3},
    std::array<float, 8>{0, 1, 0, 1, 0, -1, 0},
    std::array<float, 8>{1, 1, 1, 1, 0, -1, 2},
    std::array<float, 8>{1, 0, 1, 1, 0, -1, 1},
    std::array<float, 8>{1, 0, 0, -1, 0, 1, 3},
    std::array<float, 8>{1, 1, 0, -1, 0, 1, 0},
    std::array<float, 8>{0, 1, 1, -1, 0, 1, 2},
    std::array<float, 8>{0, 0, 1, -1, 0, 1, 1},
};

GeometryData BlockGeometry::getGeometry(const BlockId& id, BlockState* state) {
    GeometryData geometry;
    if (id >= BlockId::PLANE_GRASS) {
        return GeometryData{generationDataPlane.begin(), generationDataPlane.end()};
    }
    else {
        if (id == BlockId::BLOCK_WATER) {
            float height = 1;
            if (state != nullptr) {
                WaterBlockState* waterBlockState = reinterpret_cast<WaterBlockState*>(state);
                height = (1 - waterBlockState->level) / 8.0f;
            }

            GeometryData waterGeo;
            for (auto& data : generationDataBlock) {
                waterGeo.emplace_back(std::array<float, 8>{
                    data[0],
                    data[1] * height,
                    data[2], data[3], data[4], data[5], data[6], data[7]});
            }

            return waterGeo;
        }
        else {
            return GeometryData{generationDataBlock.begin(), generationDataBlock.end()};
        }
    }
}