#pragma once

#include "BlockIds.hpp"
#include "BlockStates/BlockState.hpp"

#include <array>
#include <glm/glm.hpp>
#include <vector>

using GeometryData = std::vector<std::array<float, 8>>;

class BlockGeometry {
  private:
    static const GeometryData generationDataBlock;
    static const GeometryData generationDataPlane;

  public:
    static GeometryData getGeometry(const BlockId& id, BlockState* state = nullptr);
};