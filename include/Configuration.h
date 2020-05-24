#pragma once

#include <map>
#include <string>

#include "BlockData.h"

typedef std::map<BlockType, BlockData> BlockDataMap;

class Configuration {
    private:
        static std::map<std::string, float> floatValues;
        static std::map<std::string, std::string> stringValues;        

        static BlockDataMap blockDataMap;        

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
};

#define CHUNK_SIZE Configuration::CHUNK_SIZE
#define CHUNK_HEIGHT Configuration::CHUNK_HEIGHT