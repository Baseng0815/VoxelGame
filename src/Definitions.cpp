#include "../include/Definitions.h"

#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::array<BlockData, (int)BlockType::NUM_BLOCKS> Definitions::BLOCK_DATA;
std::array<BiomeInfo, (int)BiomeID::NUM_BIOMES> Definitions::BIOME_INFO;

void Definitions::loadData() {
    std::ifstream file("Resources/Misc/BlockData.json");

    // not found by intellisense...actually correct
    json root = json::parse(file);

    std::string ind;
    BlockData blockData;
    int i = 0;
    for (auto it = root.begin(); it != root.end(); it++) {
        blockData.tid_up = (*it)["tid_up"].get<int>();
        blockData.tid_px = (*it)["tid_px"].get<int>();
        blockData.tid_nx = (*it)["tid_nx"].get<int>();
        blockData.tid_pz = (*it)["tid_pz"].get<int>();
        blockData.tid_nz = (*it)["tid_nz"].get<int>();
        blockData.tid_bot = (*it)["tid_bt"].get<int>();

        Definitions::BLOCK_DATA[std::stoi(it.key())] = blockData;
        i++;
    }

    file.close();

    file.open("Resources/Misc/BiomeInfo.json");

    root = json::parse(file);

    BiomeInfo biomeInfo;
    i = 0;
    for (auto it = root.begin(); it != root.end(); it++) {
        biomeInfo.baseHeight = (*it)["baseHeight"].get<int>();
        biomeInfo.heightAmplitude = (*it)["heightAmplitude"].get<int>();
        biomeInfo.id = (BiomeID)std::stoi(it.key());

        Definitions::BIOME_INFO[i] = biomeInfo;
        i++;
    }

    BLOCK_DATA.at((int)BlockType::BLOCK_ORE_COAL).oreData = OreData(20, 0, 128, 17);
    BLOCK_DATA.at((int)BlockType::BLOCK_ORE_GOLD).oreData = OreData(2, 0, 32, 9);
    BLOCK_DATA.at((int)BlockType::BLOCK_ORE_IRON).oreData = OreData(20, 0, 64, 9);
}
