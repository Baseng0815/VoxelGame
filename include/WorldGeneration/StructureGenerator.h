#pragma once
#include <glm/glm.hpp>
#include "Structures.h"

struct GenerationData;
class WorldGenerator;

class StructureGenerator {
private:
    WorldGenerator* m_worldGenerator;

    void generateStructure(const glm::vec2 &chunk, glm::vec3 position, const BlockCollection& structure, Block*** blocks) const;

    void generateTrees(const glm::vec2 &chunk, GenerationData *data) const;
public:
    StructureGenerator(WorldGenerator *worldGenerator);

    void generateStructures(const glm::vec2 &chunk, GenerationData* data) const;
};
