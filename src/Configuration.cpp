#include "../include/Configuration.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

std::map<std::string, float> Configuration::floatValues;
std::map<std::string, std::string> Configuration::stringValues;
std::map<std::string, int> Configuration::keyBinds;

BlockDataMap Configuration::blockDataMap;

using json = nlohmann::json;

void Configuration::loadConfiguration(const std::string& resourceBasePath) {
    setValue("ResourceBasePath", resourceBasePath);

    // load blockData.json
    std::ifstream file(getStringValue("ResourceBasePath") + "/Misc/BlockData.json");
    if (!file.good()) {
        std::cout << "Failed to open BlockData.json" << std::endl;
        exit(1);
    }

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
    file.open(resourceBasePath + "Misc/BiomeInfo.json");
    if (!file.good()) {
        std::cout << "Failed to open BiomeInfo.json" << std::endl;
        exit(1);
    }

    file >> root;
    file.close();

    // load Settings.json
    file.open(resourceBasePath + "/Misc/Settings.json");
    if (!file.good()) {
        std::cout << "Failed to open Settings.json" << std::endl;
        exit(1);
    }

    file >> root;
    file.close();
    floatValues = root.get<std::map<std::string, float>>();

    // load Keybinds.json
    file.open(resourceBasePath + "/Misc/Keybinds.json");
    if (!file.good()) {
        std::cout << "Failed to open Keybinds.json" << std::endl;
        exit(1);
    }

    file >> root;
    file.close();
    keyBinds = root.get<std::map<std::string, int>>();
}

float Configuration::getFloatValue(const std::string &location)
{
    return floatValues[location];
}

const std::string& Configuration::getStringValue(const std::string &location)
{
    return stringValues[location];
}

void Configuration::setValue(const std::string& location, float value)
{
    floatValues[location] = value;
}

void Configuration::setValue(const std::string& location, std::string value)
{
    stringValues[location] = value;
}

int Configuration::getAssociatedKey(const std::string &location)
{
    return keyBinds[location];
}

const BlockData& Configuration::getBlockData(BlockType block)
{
    return blockDataMap[block];
}
