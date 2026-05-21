#pragma once

#include "../../Typedefs.hpp"
#include "BlockState.hpp"

#include <glm/glm.hpp>

#include <map>
#include <set>
#include <vector>
#include <stdexcept>

struct ChunkComponent;

struct BlockStateContainer {
private:
  std::map<int, int> blockIndices;
  std::vector<BlockState *> stateData;

  static inline constexpr int getKey(const glm::ivec3 &position) {
    return getKey(position.x, position.y, position.z);
  }

  static inline constexpr int getKey(int x, int y, int z) {
    return x << 12 | y << 4 | z;
  }

  static inline constexpr glm::ivec3 getPosition(int key) {
    return glm::ivec3{(key & 0xF000) >> 12, (key & 0x0FF0) >> 4,
                      (key & 0x000F)};
  }

public:
  template <typename T>
  inline const T *operator[](const glm::ivec3 &position) const {
    return getState<T>(position);
  }

  template <typename T> inline T *operator[](const glm::ivec3 &position) {
    return getState<T>(position);
  }

  template <typename T> inline T *setBlockState(const glm::ivec3 &position) {
    int key = getKey(position);

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

  template <typename T, typename... TArgs>
  inline T *setBlockState(const glm::ivec3 &position, const TArgs &...args) {
    int key = getKey(position);

    T *state = new T(args...);
    if (blockIndices.contains(key)) {
      int index = blockIndices[key];
      delete reinterpret_cast<T *>(stateData[index]);

      stateData[index] = state;
    } else {
      blockIndices[key] = stateData.size();
      stateData.emplace_back(state);
    }

    return state;
  }

  template <typename T> inline T *getState(const glm::ivec3 &position) {
    return getState<T>(position.x, position.y, position.z);
  }

  template <typename T> inline T *getState(int x, int y, int z) {
    int key = getKey(x, y, z);

    try {
      int index = blockIndices.at(key);
      return reinterpret_cast<T *>(stateData[index]);
    } catch (std::out_of_range e) {
      return nullptr;
    }
  }

  template <typename T> inline T *setDefaultState(const glm::ivec3 &position) {
    return setBlockState<T>(position);
  }

  template <typename T>
  inline const T *getState(const glm::ivec3 &position) const {
    return getState<T>(position.x, position.y, position.z);
  }

  template <typename T> inline const T *getState(int x, int y, int z) const {
    int key = getKey(x, y, z);

    try {
      int index = blockIndices.at(key);
      return reinterpret_cast<T *>(stateData[index]);
    } catch (std::out_of_range e) {
      return nullptr;
    }
  }

  template <typename T> inline void deleteState(const glm::ivec3 &position) {
    deleteState<T>(position.x, position.y, position.z);
  }

  template <typename T> inline void deleteState(int x, int y, int z) {
    int key = getKey(x, y, z);

    if (!blockIndices.contains(key)) {
      return;
    }


    int index = blockIndices.at(key);

    delete reinterpret_cast<T *>(stateData[index]);
    stateData.erase(stateData.begin() + index);
    blockIndices.erase(key);

    for (auto &[_, i] : blockIndices) {
      if (i > index) {
        ++i;
      }
    }
  }

  BlockStateContainer &operator=(const BlockStateContainer &other);

  void deleteAll(const ChunkComponent &chunk);
};
