#include "../../include/Systems/ChunkSaveSystem.hpp"

#include "../../include/Components/ChunkComponent.hpp"
#include "../../include/Configuration.hpp"
#include "../../include/Utility/ChunkConverter.hpp"

#include <fstream>

const std::string ChunkSaveSystem::savegameDir = "./saves/world";

void ChunkSaveSystem::saveChunk(const ChunkComponent& chunk) const {
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
}