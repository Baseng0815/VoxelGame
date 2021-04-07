#pragma once

#include "noiseutils.hpp"
#include <noise/noise.h>

using namespace noise;
using namespace noise::module;

class TreeNoise {
  private:
    double m_radius = 2.5;
    Perlin m_noise;

  public:
    TreeNoise();

    void setRadius(int radius);

    bool getValue(int x, int y) const;
};
