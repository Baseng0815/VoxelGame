#pragma once
#include <glm/glm.hpp>
#include "Structures.h"

struct GenerationData;
class WorldGenerator;

class StructureGenerator {
private:
    WorldGenerator* m_worldGenerator;

    void generateStructure(glm::vec2 chunk, glm::vec3 position, const BlockCollection& structure, Block*** blocks) const;

    void generateTrees(glm::vec2 chunk, GenerationData *data) const;
public:
    StructureGenerator(WorldGenerator* worldGenerator);

    void generateStructures(glm::vec2 chunk, GenerationData* data) const;
};