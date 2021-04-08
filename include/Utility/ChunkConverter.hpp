#pragma once

#include "../Typedefs.hpp"

#include <glm/glm.hpp>
#include <vector>

struct ChunkComponent;
struct ChunkSection;

namespace Utility {
    class ChunkConverter {
      private:
        static ChunkSection getChunkSection(const ChunkComponent& chunk, const glm::ivec3& pos, int size);

      public:
        static std::vector<ChunkSection> chunkToSections(const ChunkComponent& chunk);

    };
} // namespace Utility