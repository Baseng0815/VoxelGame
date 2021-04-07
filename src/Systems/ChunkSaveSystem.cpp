#include "../../include/Systems/ChunkSaveSystem.hpp"

#include "../../include/Components/ChunkComponent.hpp"

#include <fstream>

const std::string ChunkSaveSystem::savegameDir = "./saves/world";

void ChunkSaveSystem::saveChunk(const ChunkComponent& chunk) const {
    std::string filename = savegameDir + "/chunk" + std::to_string(chunk.chunkX) + std::to_string(chunk.chunkZ) + ".bin";

    std::ofstream file{filename, std::ios::binary};
}