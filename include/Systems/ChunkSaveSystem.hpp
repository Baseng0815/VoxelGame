#pragma once

#include "System.hpp"

#include <future>
#include <string>
#include <vector>

struct ChunkComponent;

class ChunkSaveSystem : public System {
  private:
    const static std::string savegameDir;

    std::vector<std::future<void>> saveTasks;

    void saveChunk(const ChunkComponent& chunk) const;

    void _update(int dt) override;

  public:
    ChunkSaveSystem(Registry_T& registry);
};
