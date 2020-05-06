#pragma once

#include <map>
#include <string>

#include "BlockData.h"
#include "BiomeInfo.h"

typedef std::map<BlockType, BlockData> BlockDataMap;
typedef std::map<BiomeID, BiomeInfo> BiomeInfoMap;

class Configuration {
    private:
        static std::map<std::string, float> floatValues;
        static std::map<std::string, std::string> stringValues;

        static BlockDataMap blockDataMap;
        static BiomeInfoMap biomeInfoMap;

    public:
        static constexpr int CHUNK_SIZE = 16;
        static constexpr int CHUNK_HEIGHT = 256;
        static constexpr int CHUNK_PRELOAD_SIZE = 8;
        static constexpr int CHUNK_MAX_LOADING = 6;
        static constexpr int MAP_SEED_SIZE=256;

        // path contains settings.json, blockData.json and biomeInfo.json
        static void loadConfiguration(const std::string& baseResourcePath);

        static float getFloatValue(const std::string& location);
        static const std::string& getStringValue(const std::string& location);
        static void setValue(const std::string& location, float value);
        static void setValue(const std::string& location, std::string value);

        static const BlockData& getBlockData(BlockType block);
        static const BiomeInfo& getBiomeInfo(BiomeID biome);
};
