#pragma once

#include "System.hpp"

#include <future>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <vector>

struct ChunkComponent;
struct BlockChangedEvent;
struct Block;
struct FluidBlockState;

class FluidSystem : public System {
private:
  CallbackHandle<const BlockChangedEvent &> m_blockChangedHandle;
  void handleBlockChanged(const BlockChangedEvent &e) const;

  void spreadFluid(const glm::ivec3 &position, ChunkComponent &chunk) const;

  void updateWater(int dt, ChunkComponent &chunk) const;

  void _update(int dt) override;

public:
  FluidSystem(Registry_T &registry);
};
