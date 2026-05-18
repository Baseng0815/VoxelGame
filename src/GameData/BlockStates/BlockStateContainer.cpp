#include "../../../include/GameData/BlockStates/BlockStateContainer.hpp"

#include "../../../include/GameData/BlockStates/WaterBlockState.hpp"

#include <stdexcept>

BlockStateContainer& BlockStateContainer::operator=(const BlockStateContainer& other) {
    this->blockIndices = other.blockIndices;
    this->stateData = other.stateData;
    return *this;
}

void BlockStateContainer::deleteBlockState(const glm::vec3& position) {
    try {
        int key = (int)position.x << 12 | (int)position.y << 4 | (int)position.z;
        int index = blockIndices.at(key);
        delete stateData[index];
        stateData.erase(stateData.begin() + index);
        blockIndices.erase(key);
    }
    catch (std::out_of_range e) {
    }
}

