#pragma once

#include "Configuration.h"

typedef std::array<std::array<int, Configuration::CHUNK_SIZE>, Configuration::CHUNK_SIZE> ChunkMapArray;

class MapGenerator {
    private:
        int octavesCount = 5;
        float terrainSeed[Configuration::MAP_SEED_SIZE * Configuration::MAP_SEED_SIZE];

        int maxTerrainHeight = 60;
        int minTerrainHeight = 10;

        float noise(int x, int y);
    public:
        void generateSeed();

        ChunkMapArray&& generateChunkMap(int x, int y);
};

