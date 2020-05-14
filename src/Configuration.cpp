#include "../include/Configuration.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

std::map<std::string, float> Configuration::floatValues;
std::map<std::string, std::string> Configuration::stringValues;
BlockDataMap Configuration::blockDataMap;

using json = nlohmann::json;

void Configuration::loadConfiguration(const std::string& baseResourcePath) {
    setValue("ResourceBasePath", baseResourcePath);

    // load blockData.json
    std::ifstream file(getStringValue("ResourceBasePath") + "/Misc/BlockData.json");
    if (!file.good())
        std::cout << "Failed to open BlockData.json" << std::endl;

    json root;
    file >> root;
    file.close();

    for (auto it = root.begin(); it != root.end(); it++) {
        BlockData blockData;
        blockData.tid_up = (*it)["tid_up"].get<int>();
        blockData.tid_px = (*it)["tid_px"].get<int>();
        blockData.tid_nx = (*it)["tid_nx"].get<int>();
        blockData.tid_pz = (*it)["tid_pz"].get<int>();
        blockData.tid_nz = (*it)["tid_nz"].get<int>();
        blockData.tid_bot = (*it)["tid_bt"].get<int>();

        blockDataMap[(BlockType)std::stoi(it.key())] = blockData;
    }

    blockDataMap[BlockType::BLOCK_ORE_COAL].oreData = OreData(20, 0, 128, 17);
    blockDataMap[BlockType::BLOCK_ORE_GOLD].oreData = OreData(2, 0, 32, 9);
    blockDataMap[BlockType::BLOCK_ORE_IRON].oreData = OreData(20, 0, 64, 9);

    // load biomeInfo.json
    file.open(baseResourcePath + "/Misc/BiomeInfo.json");
    if (!file.good())
        std::cout << "Failed to open BiomeInfo.json" << std::endl;

    file >> root;
    file.close();    

    // load other configuration data
    file.open(baseResourcePath + "/Misc/Settings.json");
    if (!file.good())
        std::cout << "Failed to open Settings.json" << std::endl;

    file >> root;
    file.close();
    floatValues = root.get<std::map<std::string, float>>();
}

float Configuration::getFloatValue(const std::string& location) {
    return floatValues[std::string(location)];
}

const std::string& Configuration::getStringValue(const std::string& location) {
    return stringValues[std::string(location)];
}

void Configuration::setValue(const std::string& location, float value) {
    floatValues[std::string(location)] = value;
}

void Configuration::setValue(const std::string& location, std::string value) {
    stringValues[std::string(location)] = value;
}

const BlockData& Configuration::getBlockData(BlockType block) {
    return blockDataMap[block];
}

