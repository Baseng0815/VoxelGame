#pragma once

#include "System.hpp"

struct ChunkComponent;
struct BlockChangedEvent;
struct Block;
struct WaterBlockState;

class ChunkUpdateSystem : public System {
  private:
    void updateFluids(ChunkComponent& chunk, int dt);

    static void updateWater(ChunkComponent& chunk, Block& block, WaterBlockState& waterState, bool waterOnTop = false);

    void _update(int dt) override;    

  public:
    ChunkUpdateSystem(Registry_T& registry);
};