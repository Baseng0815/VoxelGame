#pragma once
#include <glm/glm.hpp>
#include "Structures.h"

struct GenerationData;

class StructureGenerator {
private:   
    void generateStructure(glm::vec2 chunk, glm::vec3 position, const BlockCollection& structure, Block*** blocks) const;
public:
    StructureGenerator();

    void generateStructures(glm::vec2 chunk, GenerationData* data) const;
};