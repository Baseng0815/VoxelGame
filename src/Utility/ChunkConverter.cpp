#include "../../include/Utility/ChunkConverter.hpp"

#include "../../include/Components/ChunkComponent.hpp"
#include "../../include/Configuration.hpp"
#include "../../include/GameData/ChunkSection.hpp"

namespace Utility {
    ChunkSection ChunkConverter::getChunkSection(const ChunkComponent& chunk, const glm::ivec3& position, int size) {
        if (size == 1) {
            return ChunkSection{position, (byte)1, chunk.blocks[position.x][position.y][position.z]};
        }

        // get subsections data
        int halfSize = size / 2;
        ChunkSection sections[ChunkSection::SUBSECTIONS_COUNT];

        for (int i = 0; i < ChunkSection::SUBSECTIONS_COUNT; i++) {
            glm::ivec3 subsectionPos = position + halfSize * ChunkSection::getDirection(i);
            sections[i] = getChunkSection(chunk, subsectionPos, halfSize);
        }

        ChunkSection section{position, (byte)size, sections};
        section.cleanUp();
        return section;
    }

    std::vector<ChunkSection> ChunkConverter::chunkToSections(const ChunkComponent& chunk) {
        constexpr int sectionsCount = Configuration::CHUNK_HEIGHT / Configuration::CHUNK_SIZE;
        std::vector<ChunkSection> sections{sectionsCount};

        for (int i = 0; i < sectionsCount; i++) {
            sections.emplace_back(getChunkSection(chunk, Configuration::CHUNK_SIZE * i * glm::ivec3{0, 1, 0}, Configuration::CHUNK_SIZE));
        }

        sections.shrink_to_fit();
        return sections;
    }
} // namespace Utility