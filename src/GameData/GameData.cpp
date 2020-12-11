#include "../../include/GameData/GameData.hpp"

#include "../../include/Configuration.hpp"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::vector<BlockTemplate> GameData::m_blockTemplates;
std::vector<Structure> GameData::m_structures;

void GameData::loadGameData()
{
    // load blockData.json
    std::ifstream stream {Configuration::getStringValue("ResourceBasePath") + "/Misc/BlockData.json"};
    if (!stream.good()) {
        std::cout << "Failed to open BlockData.json" << std::endl;
        exit(1);
    }

    json root;
    stream >> root;
    stream.close();

    for (auto it = root.begin(); it != root.end(); it++) {
        BlockTemplate blockTemplate;
        blockTemplate.name          = (*it)["name"].get<std::string>();
        blockTemplate.isSolid       = (*it)["is_solid"].get<bool>();
        blockTemplate.isTransparent = (*it).contains("is_transparent") ? (*it)["is_transparent"].get<bool>() : false;
        blockTemplate.tid_py        = (*it)["tid_py"].get<int>();
        blockTemplate.tid_px        = (*it)["tid_px"].get<int>();
        blockTemplate.tid_nx        = (*it)["tid_nx"].get<int>();
        blockTemplate.tid_pz        = (*it)["tid_pz"].get<int>();
        blockTemplate.tid_nz        = (*it)["tid_nz"].get<int>();
        blockTemplate.tid_ny        = (*it)["tid_ny"].get<int>();
        std::cout << blockTemplate.name << std::endl;

        m_blockTemplates.emplace_back(std::move(blockTemplate));
    }

    m_blockTemplates[(size_t)BlockId::BLOCK_ORE_COAL].oreData = OreData {20, 0, 128, 17};
    m_blockTemplates[(size_t)BlockId::BLOCK_ORE_GOLD].oreData = OreData {2, 0, 32, 9};
    m_blockTemplates[(size_t)BlockId::BLOCK_ORE_IRON].oreData = OreData {20, 0, 64, 9};

    // load biomeInfo.json
    stream.open(Configuration::getStringValue("ResourceBasePath") + "Misc/BiomeInfo.json");
    if (!stream.good()) {
        std::cout << "Failed to open BiomeInfo.json" << std::endl;
        exit(1);
    }

    stream >> root;
    stream.close();

    // load structures
    stream.open(Configuration::getStringValue("ResourceBasePath") + "Misc/Structures.json");
    if(!stream.good()) {
        std::cout << "Failed to open Structures.json" << std::endl;
        exit(1);
    }

    stream >> root;
    stream.close();

    for(auto it = root.begin(); it != root.end(); it++) {
        Structure structure;
        structure.type = (StructureId)(*it)["type"].get<short>();
        structure.variation = (*it)["type"].get<int>();

        for(auto jt = (*it)["blocks"].begin(); jt!= (*it)["blocks"].end(); jt++) {
            Section section;
            section.block_type = (BlockId)(*jt)["blockId"].get<short>();
            section.position0 = glm::ivec3{
                (*jt)["position0"][0].get<int>(),
                (*jt)["position0"][1].get<int>(),
                (*jt)["position0"][2].get<int>()
            };
            section.position1 = glm::ivec3{
                (*jt)["position1"][0].get<int>(),
                (*jt)["position1"][1].get<int>(),
                (*jt)["position1"][2].get<int>()
            };

            structure.sections.emplace_back(std::move(section));
        }        

        m_structures.emplace_back(std::move(structure));
    }
}

const BlockTemplate &GameData::getBlockTemplate(BlockId block)
{
    return m_blockTemplates[(size_t)block];
}

const Structure& GameData::getStructure(StructureId structureId, int variation) {
    for(const auto& structure : m_structures) {
        if(structure.type == structureId && structure.variation == variation) {
            return structure;
        }
    }
}
