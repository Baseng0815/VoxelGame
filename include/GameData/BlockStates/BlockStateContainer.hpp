#pragma once

#include "../../Typedefs.hpp"
#include "BlockState.hpp"

#include <glm/glm.hpp>

#include <map>
#include <vector>

struct BlockStateContainer {
  private:
    std::map<int, int> blockIndices;
    std::vector<BlockState*> stateData;

  public:
    template<typename T>
    T* createBlockState(const glm::vec3& position);
    
    void deleteBlockState(const glm::vec3& position);

    template<typename T>
    T* getState(const glm::vec3& position);

    template<typename T>
    void setDefaultState(const glm::vec3& position);

    template<typename T>
    const T* getState(const glm::vec3& position) const;

    BlockStateContainer& operator=(const BlockStateContainer& other);
};
