#pragma once

#include "../../Typedefs.hpp"
#include "BlockState.hpp"

#include <glm/glm.hpp>

#include <map>
#include <vector>

struct BlockStateContainer {
private:
  std::map<int, int> blockIndices;
  std::vector<BlockState *> stateData;

public:
  template <typename T> inline T *createBlockState(const glm::vec3 &position) {
    int key = (int)position.x << 12 | (int)position.y << 4 | (int)position.z;

    T *state = new T{};
    if (blockIndices.contains(key)) {
      int index = blockIndices[key];
      delete stateData[index];

      stateData[index] = state;
    } else {
      blockIndices[key] = stateData.size();
      stateData.emplace_back(state);
    }

    return state;
  }

  void deleteBlockState(const glm::vec3 &position);

  template <typename T> inline T *getState(const glm::vec3 &position) {
    int key = (int)position.x << 12 | (int)position.y << 4 | (int)position.z;
    int index = blockIndices[key];
    return reinterpret_cast<T *>(stateData[index]);
  }

  template <typename T>
  inline void setDefaultState(const glm::vec3 &position) {}

  template <typename T>
  inline const T *getState(const glm::vec3 &position) const {
    int key = (int)position.x << 12 | (int)position.y << 4 | (int)position.z;
    int index = blockIndices.at(key);
    return reinterpret_cast<T *>(stateData[index]);
  }

  BlockStateContainer &operator=(const BlockStateContainer &other);
};
