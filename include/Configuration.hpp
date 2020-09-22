#pragma once

#include <map>
#include <string>

struct Event;

class Configuration {
    private:
        static std::map<std::string, float> floatValues;
        static std::map<std::string, std::string> stringValues;
        static std::map<std::string, int> keyBinds;

    public:
        static constexpr int CHUNK_SIZE = 16;
        static constexpr int CHUNK_HEIGHT = 256;
        static constexpr int CHUNK_PRELOAD_SIZE = 6;
        static constexpr int CHUNK_COUNT_PER_AXIS = CHUNK_PRELOAD_SIZE * 2 + 1;
        static constexpr int CHUNK_MAX_LOADING = 6;
        static constexpr int MAP_SEED_SIZE = 256;
        static constexpr int SKYBOX_SIZE = 4000;
        // units per second
        static constexpr float CLOUD_SPEED = 16;
        // the higher the bias, the more clouds there will be
        static constexpr double CLOUD_BIAS = 0.0;
        static constexpr float CLOUD_HEIGHT = 120;
        static constexpr float CLOUD_VOLUMETRIC_HEIGHT = 8;

        // resourceBasePath contains all resources and static data
        static void loadConfiguration(const std::string &resourceBasePath);

        static float getFloatValue(const std::string &location);
        static const std::string& getStringValue(const std::string &location);
        static void setValue(const std::string &location, float value);
        static void setValue(const std::string &location, std::string value);

        static int getAssociatedKey(const std::string &location);
};
