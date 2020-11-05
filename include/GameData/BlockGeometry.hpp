#pragma once

#include "Block.hpp"

#include <array>
#include <glm/glm.hpp>
#include <vector>

using GeometryData = std::vector<std::array<float, 8>>;

class BlockGeometry {
  private:
    static const GeometryData generationDataBlock;
    static const GeometryData generationDataPlane;

  public:
    static GeometryData getGeometry(const BlockId& type, const BlockState* state);
};