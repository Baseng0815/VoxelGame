#include "../../include/Systems/ChunkSaveSystem.hpp"

#include "../../include/Components/ChunkComponent.hpp"
#include "../../include/Configuration.hpp"
#include "../../include/Utility/ChunkConverter.hpp"

#include <fstream>

const std::string ChunkSaveSystem::savegameDir = "./saves/world";

entt::entity ChunkSaveSystem::saveChunk(const entt::entity entity, const ChunkComponent& chunk) {
    std::string filename = savegameDir + "/chunk" + std::to_string(chunk.chunkX) + std::to_string(chunk.chunkZ) + ".bin";

    std::ofstream file{filename, std::ios::binary};

    std::vector<ChunkSection> chunkData = Utility::ChunkConverter::chunkToSections(chunk);

    for (int i = 0; i < Configuration::CHUNK_HEIGHT / Configuration::CHUNK_SIZE; i++) {
        auto& section = chunkData[i];
        if (!section.isAir()) {
            size_t length = 0;
            const char* data = section.getData(&length);

            file << (byte)i;
            file.write(data, length);

            delete[] data;
        }
    }

    file.flush();
    file.close();

    return entity;
}

void ChunkSaveSystem::_update(int dt) {
    // check for ended save tasks
    for (auto it = saveTasks.begin(); it != saveTasks.end(); it++) {
        if (it->valid() && it->wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            entt::entity entity = it->get();
            ChunkComponent& chunk = m_registry.get<ChunkComponent>(entity);

            // remove save requested state
            chunk.state ^= ChunkComponent::States::SAVE_REQUESTED;

            it = saveTasks.erase(it);
        }
    }

    // add new save tasks if saves requested
    while (saveTasks.size() < Configuration::MAX_CHUNK_SAVE_TASKS && saveQueue.size() > 0) {
        entt::entity chunkEntity = saveQueue.front();
        const ChunkComponent& chunk = m_registry.get<ChunkComponent>(chunkEntity);

        // saveTasks.push_back(std::async(std::launch::async, ChunkSaveSystem::saveChunk, chunkEntity, chunk));
        saveChunk(chunkEntity, chunk);

        saveQueue.pop();
    }

    // check for chunks which should be saved
    auto view = m_registry.view<ChunkComponent>();
    for (auto entity : view) {
        const ChunkComponent& chunk = view.get(entity);
        if (chunk.state & ChunkComponent::States::SAVE_REQUESTED) {
            saveQueue.push(entity);
        }
    }
}

ChunkSaveSystem::ChunkSaveSystem(Registry_T& registry)
    : System{registry} {
}
