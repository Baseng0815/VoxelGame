#pragma once

#include <map>
#include <string>

#include "BlockData.h"

typedef std::map<BlockType, BlockData> BlockDataMap;

struct Event;

class Configuration {
    private:
        static std::map<std::string, float> floatValues;
        static std::map<std::string, std::string> stringValues;

        static BlockDataMap blockDataMap;

    public:
        static constexpr int CHUNK_SIZE = 16;
        static constexpr int CHUNK_HEIGHT = 256;
        // TODO finish errors when CHUNK_PRELOAD_SIZE is 0 or 1
        static constexpr int CHUNK_PRELOAD_SIZE = 6;
        static constexpr int CHUNK_MAX_LOADING = 6;
        static constexpr int MAP_SEED_SIZE=256;

        // path contains settings.json, blockData.json and biomeInfo.json
        static void loadConfiguration(const std::string& resourceBasePath);

        static float getFloatValue(const std::string& location);
        static const std::string& getStringValue(const std::string& location);
        static void setValue(const std::string& location, float value);
        static void setValue(const std::string& location, std::string value);

        static const BlockData& getBlockData(BlockType block);
};
