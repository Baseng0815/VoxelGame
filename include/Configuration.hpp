#pragma once

#include <map>
#include <string>

struct Event;

// this class contains settings and compile-time constants
// TODO maybe move settings out of here
class Configuration {
    private:
        static std::map<std::string, float> floatValues;
        static std::map<std::string, std::string> stringValues;
        static std::map<std::string, int> keyBinds;

    public:
        static constexpr int CHUNK_SECTION_SIZE = 16;
        static constexpr int CHUNK_SIZE         = 16;
        static constexpr int CHUNK_HEIGHT       = 256;
        static constexpr int CHUNK_PRELOAD_SIZE = 6;
        static constexpr int CHUNK_COUNT_PER_AXIS = CHUNK_PRELOAD_SIZE * 2 + 1;
        static constexpr int CHUNK_MAX_LOADING  = 6;
        static constexpr int MAP_SEED_SIZE      = 256;
        static constexpr int SKYBOX_SIZE        = 4000;
        static constexpr int MAX_POINT_LIGHTS   = 10;
        static constexpr int MAX_DIR_LIGHTS     = 1;
        // in real seconds
        static constexpr int DAYNIGHT_DURATION  = 100;
        // units per second
        static constexpr float CLOUD_SPEED      = 16;
        // in milliseconds
        static constexpr int KEY_INTERVAL       = 200;
        // the higher the bias, the more clouds there will be
        static constexpr double CLOUD_BIAS      = 0.0;
        static constexpr float CLOUD_HEIGHT     = 120;
        static constexpr float CLOUD_VOLUMETRIC_HEIGHT = 8;

        // the time water needs to move 1 block
        static constexpr float WATER_FLOW_TIME = 4.0f;

        // resourceBasePath contains all resources and static data
        static void loadConfiguration(const std::string &resourceBasePath);

        static float getFloatValue(const std::string &location);
        static const std::string& getStringValue(const std::string &location);
        static void setValue(const std::string &location, float value);
        static void setValue(const std::string &location, std::string value);

        static int getAssociatedKey(const std::string &location);
};
