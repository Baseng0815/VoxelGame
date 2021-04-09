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

    void ChunkConverter::fillBlocks(BlockId*** blocks, const ChunkSection& section) {
        if (section.isAir()) {
            return;
        }

        if (!section.hasSubsections()) {
            BlockId block = section.blockType;

            glm::ivec3 start = section.position;
            glm::ivec3 end = start + (int)section.size * glm::ivec3{1};

            for (int x = start.x; x < end.x; x++) {
                for (int y = start.y; y < end.y; y++) {
                    for (int z = start.z; z < end.z; z++) {
                        blocks[x][y][z] = block;
                    }
                }
            }
        }
        else {
            int halfSize = (int)section.size / 2;
            for (int i = 0; i < ChunkSection::SUBSECTIONS_COUNT; i++) {
                const ChunkSection& subsection = section.subsections[i];
                fillBlocks(blocks, subsection);
            }
        }
    }

    std::vector<ChunkSection> ChunkConverter::chunkToSections(const ChunkComponent& chunk) {
        std::vector<ChunkSection> sections;
        sections.reserve(sectionsCount);

        for (int i = 0; i < sectionsCount; i++) {
            sections.push_back(getChunkSection(chunk, Configuration::CHUNK_SIZE * i * glm::ivec3{0, 1, 0}, Configuration::CHUNK_SIZE));
        }

        return sections;
    }

    BlockId*** ChunkConverter::sectionsToChunk(const std::vector<ChunkSection>& sections) {
        BlockId*** blocks = new BlockId**[Configuration::CHUNK_SIZE];
        for (int x = 0; x < Configuration::CHUNK_SIZE; x++) {
            blocks[x] = new BlockId*[Configuration::CHUNK_HEIGHT];
            for (int y = 0; y < Configuration::CHUNK_HEIGHT; y++) {
                blocks[x][y] = new BlockId[Configuration::CHUNK_SIZE];
            }
        }

        for (const auto& section : sections) {
            fillBlocks(blocks, section);
        }

        return blocks;
    }
} // namespace Utility