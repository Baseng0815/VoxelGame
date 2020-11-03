#include "../../../include/GameData/BlockStates/BlockStateContainer.hpp"

#include <stdexcept>

template<>
WaterBlockState& BlockStateContainer::createBlockState<WaterBlockState>(const glm::vec3& position) {
    blockIndices[position] = waterStates.size();
    return waterStates.emplace_back(true);
}

template<>
void BlockStateContainer::deleteBlockState<WaterBlockState>(const glm::vec3& position) {
    try {
        int index = blockIndices.at(position);
        waterStates.erase(waterStates.begin() + index);
        blockIndices.erase(position);
    }
    catch (std::out_of_range e) {
    }
}

template<>
WaterBlockState& BlockStateContainer::getState<WaterBlockState>(const glm::vec3& position) {
    if (blockIndices.contains(position)) {
        int index = blockIndices[position];
        return waterStates[index];
    }
    else {
        return waterStates[0];
    }
}

template<>
const WaterBlockState& BlockStateContainer::getState<WaterBlockState>(const glm::vec3& position) const {
    if (blockIndices.contains(position)) {
        int index = blockIndices.at(position);
        return waterStates[index];
    }
    else {
        return waterStates[0];
    }
}

template<>
WaterBlockState& BlockStateContainer::operator[](const glm::vec3& position) {
    return getState<WaterBlockState>(position);
}

template<>
const WaterBlockState& BlockStateContainer::operator[](const glm::vec3& position) const {
    return getState<WaterBlockState>(position);
}
