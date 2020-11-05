#pragma once

#include "System.hpp"

#include <future>
#include <vector>

struct ChunkComponent;
struct BlockChangedEvent;
struct Block;
struct WaterBlockState;

class ChunkUpdateSystem : public System {
  private:
    CallbackHandle<const BlockChangedEvent&> m_blockHandle;
    void handleBlockChanged(const BlockChangedEvent& e);    

    void updateWater(ChunkComponent& chunk, const int& x, const int& y, const int& z);

    void _update(int dt) override;

    std::vector<std::future<void>> m_updateFutures;

  public:
    ChunkUpdateSystem(Registry_T& registry);
};