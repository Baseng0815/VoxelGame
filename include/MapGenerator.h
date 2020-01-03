#pragma once

#include "Chunk.h"
#include "Definitions.h"

typedef std::array<std::array<int, Definitions::CHUNK_SIZE>, Definitions::CHUNK_SIZE> ChunkMapArray;

class MapGenerator {
    private:
        int octavesCount = 5;
        float terrainSeed[Definitions::MAP_SEED_SIZE * Definitions::MAP_SEED_SIZE];

        int maxTerrainHeight = 60;
        int minTerrainHeight = 10;

        float noise(int x, int y);
    public:
        void generateSeed();

        ChunkMapArray&& generateChunkMap(int x, int y);
};

