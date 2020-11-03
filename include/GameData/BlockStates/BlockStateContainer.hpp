#pragma once

#include "../../Typedefs.hpp"
#include "WaterBlockState.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include <unordered_map>
#include <vector>

struct BlockStateContainer {
  private:
    std::unordered_map<glm::vec3, int, std::hash<glm::vec3>> blockIndices;
    std::vector<WaterBlockState> waterStates = {WaterBlockState{false, 0}};

  public:
    template<typename T>
    T& createBlockState(const glm::vec3& position);

    template<typename T>
    void deleteBlockState(const glm::vec3& position);

    template<typename T>
    T& getState(const glm::vec3& position);

    template<typename T>
    const T& getState(const glm::vec3& position) const;

    template<typename T>
    T& operator[](const glm::vec3& position);

    template<typename T>
    const T& operator[](const glm::vec3& position) const;
};
