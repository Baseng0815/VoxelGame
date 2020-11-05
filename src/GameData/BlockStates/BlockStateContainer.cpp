#include "../../../include/GameData/BlockStates/BlockStateContainer.hpp"

#include "../../../include/GameData/BlockStates/WaterBlockState.hpp"

#include <stdexcept>

BlockStateContainer& BlockStateContainer::operator=(const BlockStateContainer& other) {
    this->blockIndices = other.blockIndices;
    this->stateData = other.stateData;
    return *this;
}

template<typename T>
T* BlockStateContainer::createBlockState(const glm::vec3& position) {
    int key = (int)position.x << 12 | (int)position.y << 4 | (int)position.z;
    
    T* state = new T{};
    if (blockIndices.contains(key)) {
        int index = blockIndices[key];
        delete stateData[index];

        stateData[index] = state;
    }
    else {
        blockIndices[key] = stateData.size();
        stateData.emplace_back(state);
    }

    return state;
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

template<typename T>
T* BlockStateContainer::getState(const glm::vec3& position) {
    int key = (int)position.x << 12 | (int)position.y << 4 | (int)position.z;
    int index = blockIndices[key];
    return reinterpret_cast<T*>(stateData[index]);
}

template<typename T>
const T* BlockStateContainer::getState(const glm::vec3& position) const {
    int key = (int)position.x << 12 | (int)position.y << 4 | (int)position.z;
    int index = blockIndices.at(key);
    return reinterpret_cast<T*>(stateData[index]);
}

template WaterBlockState* BlockStateContainer::createBlockState<WaterBlockState>(const glm::vec3&);
template WaterBlockState* BlockStateContainer::getState<WaterBlockState>(const glm::vec3&);
template const WaterBlockState* BlockStateContainer::getState<WaterBlockState>(const glm::vec3&) const;
