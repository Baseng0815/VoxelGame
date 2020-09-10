#include "../../include/WorldGeneration/CaveGenerator.h"

#include "../../include/Block.h"
#include "../../include/Configuration.h"
#include "../../include/Components/WorldComponent.h"
#include "../../include/WorldGeneration/PerlinWorm.h"

#include <iostream>

void CaveGenerator::createWorm(glm::vec2 chunk) {
    glm::vec3 head = glm::vec3(8, 32, 8);

    glm::vec3 worldCoords = Utility::GetWorldCoords(chunk, head);

    PerlinWorm worm = PerlinWorm(cavePerlin, worldCoords, 16, 128);
    m_worms.push_back(worm);

    std::vector<WormPart> chunkWorms;
    DivideWorm(worm, chunkWorms);

    for(WormPart chunkWorm : chunkWorms) {
        try {
            auto wormColl = m_cache.at(chunkWorm.chunk);
            wormColl.push_back(chunkWorm);
        }
        catch (std::out_of_range) {
            m_cache.insert_or_assign(chunkWorm.chunk, std::vector<WormPart>{chunkWorm});
        }
    }
}

CaveGenerator::CaveGenerator() {
    cavePerlin.SetFrequency(0.5);
    cavePerlin.SetOctaveCount(3);
    cavePerlin.SetPersistence(2);
    cavePerlin.SetLacunarity(3);

    createWorm(glm::vec2(0, 0));
}

void CaveGenerator::generate(glm::vec2 position, Block*** blocks) {
    /*for(int x = -1; x <= 1; x++)
      for(int z = -1; z <= 1; z++) {
      try {
      glm::vec2 chunk = position + glm::vec2(x, z);
      std::vector<WormPart>& parts = m_cache.at(chunk);

    // foreach worm part
    for(auto part : parts) {
    // foreach segment
    glm::vec3 pos = part.head + glm::vec3(x * CHUNK_SIZE, 0, z * CHUNK_SIZE);
    for(auto seg : part.segments) {
    glm::vec3 blockDir = glm::normalize(seg) * (float)sqrt(2);
    glm::vec3 relPos = glm::vec3();

    // foreach block in segment
    for(int i = 0; i < length(seg); i++) {
    FillSphere(pos + relPos, 5, blocks, (char)BlockType::BLOCK_STONE);
    relPos += blockDir;
    }
    }
    }
    }
    catch (std::out_of_range) {

    }
    }*/
    for (int x = -1; x <= 1; x++)
        for (int z = -1; z <= 1; z++) {
            try {
                glm::vec2 chunk = position + glm::vec2(x, z);
                for(auto worm : m_worms) {

                }
            }
            catch (std::out_of_range) {
            }
        }
}

