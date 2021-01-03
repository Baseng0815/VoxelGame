#pragma once

#include "System.hpp"

#include <glm/glm.hpp>
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

        void updateWater(const glm::ivec3& position);

        void _update(int dt) override;

        std::vector<std::future<void>> m_updateFutures;
        std::vector<std::pair<float, glm::ivec3>> m_waterUpdates;

      public:
        ChunkUpdateSystem(Registry_T& registry);
};
