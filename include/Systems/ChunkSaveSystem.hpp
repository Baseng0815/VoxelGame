#pragma once

#include "System.hpp"

#include <future>
#include <queue>
#include <string>
#include <vector>

struct ChunkComponent;

class ChunkSaveSystem : public System {
  private:
    const static std::string savegameDir;

    std::queue<entt::entity> saveQueue;
    std::vector<std::future<entt::entity>> saveTasks;

    static entt::entity saveChunk(const entt::entity entity, const ChunkComponent& chunk);

    void _update(int dt) override;

  public:
    ChunkSaveSystem(Registry_T& registry);
};
