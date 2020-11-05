#include "../../../include/GameData/BlockStates/BlockStateContainer.hpp"

#include <stdexcept>

BlockStateContainer& BlockStateContainer::operator=(const BlockStateContainer& other) {
    this->blockIndices = other.blockIndices;
    this->waterStates = other.waterStates;
    return *this;
}

template<>
WaterBlockState* BlockStateContainer::createBlockState<WaterBlockState>(const glm::vec3& position) {
    int key = (int)position.x << 12 | (int)position.y << 4 | (int)position.z;
    blockIndices[key] = waterStates.size();
    WaterBlockState* state = new WaterBlockState{};
    return waterStates.emplace_back(state);
}

template<>
void BlockStateContainer::deleteBlockState<WaterBlockState>(const glm::vec3& position) {
    try {
        int key = (int)position.x << 12 | (int)position.y << 4 | (int)position.z;
        int index = blockIndices.at(key);
        delete waterStates[index];
        waterStates.erase(waterStates.begin() + index);
        blockIndices.erase(key);
    }
    catch (std::out_of_range e) {
    }
}

template<>
WaterBlockState* BlockStateContainer::getState<WaterBlockState>(const glm::vec3& position) {
    int key = (int)position.x << 12 | (int)position.y << 4 | (int)position.z;
    int index = blockIndices[key];
    return waterStates[index];
}

template<>
const WaterBlockState* BlockStateContainer::getState<WaterBlockState>(const glm::vec3& position) const {
    int key = (int)position.x << 12 | (int)position.y << 4 | (int)position.z;
    int index = blockIndices.at(key);
    return waterStates[index];
}
