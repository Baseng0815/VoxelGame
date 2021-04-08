#pragma once

#include "../Typedefs.hpp"

#include <glm/glm.hpp>
#include <vector>

struct ChunkComponent;
struct ChunkSection;

namespace Utility {
    class ChunkConverter {
      private:
        static constexpr int sectionsCount = Configuration::CHUNK_HEIGHT / Configuration::CHUNK_SIZE;

        static ChunkSection getChunkSection(const ChunkComponent& chunk, const glm::ivec3& pos, int size);

        static void fillBlocks(BlockId*** blocks, const ChunkSection& section);

      public:
        static std::vector<ChunkSection> chunkToSections(const ChunkComponent& chunk);

        static BlockId*** sectionsToChunk(const std::vector<ChunkSection>& sections);
    };
} // namespace Utility