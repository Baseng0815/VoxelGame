#pragma once

#include "../../Typedefs.hpp"
#include "BlockState.hpp"

#include <glm/glm.hpp>

#include <map>
#include <vector>

struct ChunkComponent;

struct BlockStateContainer {
private:
  std::map<int, int> blockIndices;
  std::vector<BlockState *> stateData;

  static inline constexpr int getKey(const glm::vec3 &position) {
    return getKey(position.x, position.y, position.z);
  }

  static inline constexpr int getKey(float x, float y, float z) {
    return static_cast<int>(x) << 12 | static_cast<int>(y) << 4 |
           static_cast<int>(z);
  }

  static inline constexpr glm::ivec3 getPosition(int key) {
    return glm::ivec3{(key & 0xF000) >> 12, (key & 0x0FF0) >> 4,
                      (key & 0x000F)};
  }

public:
  template <typename T>
  inline const T *operator[](const glm::vec3 &position) const {
    return getState<T>(position);
  }

  template <typename T> inline T *operator[](const glm::vec3 &position) {
    return getState<T>(position);
  }

  template <typename T> inline T *createBlockState(const glm::vec3 &position) {
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
  inline T *createBlockState(const glm::vec3 &position, const TArgs &...args) {
    int key = getKey(position);

    T *state = new T(args...);
    if (blockIndices.contains(key)) {
      int index = blockIndices[key];
      delete reinterpret_cast<T *>(stateData[index]);

      stateData[index] = state;
    }
    else {
      blockIndices[key] = stateData.size();
      stateData.emplace_back(state);
    }

    return state;
  }

  template <typename T> inline T *getState(const glm::vec3 &position) {
    return getState<T>(position.x, position.y, position.z);
  }

  template <typename T> inline T *getState(float x, float y, float z) {
    int key = getKey(x, y, z);
    int index = blockIndices[key];
    return reinterpret_cast<T *>(stateData[index]);
  }

  template <typename T>
  inline void setDefaultState(const glm::vec3 &position) {}

  template <typename T>
  inline const T *getState(const glm::vec3 &position) const {
    return getState<T>(position.x, position.y, position.z);
  }

  template <typename T>
  inline const T *getState(float x, float y, float z) const {
    int key = getKey(x, y, z);
    int index = blockIndices.at(key);
    return reinterpret_cast<T *>(stateData[index]);
  }

  BlockStateContainer &operator=(const BlockStateContainer &other);

  void deleteAll(const ChunkComponent &chunk);
};
