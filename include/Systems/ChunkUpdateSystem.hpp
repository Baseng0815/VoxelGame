#pragma once

#include "System.hpp"

struct ChunkComponent;

class ChunkUpdateSystem : public System {
  private:
    void updateFluids(ChunkComponent& chunk, int dt);

    void updateWater(ChunkComponent& chunk, int x, int y, int z);

    void _update(int dt) override;

  public:
    ChunkUpdateSystem(Registry_T& registry);
};