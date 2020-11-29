#pragma once
#include "BlockIds.hpp"

#include <glm/glm.hpp>
#include <vector>

enum class StructureId : short {
    STRUCTURE_TREE
};

struct Section {
    BlockId block_type;
    glm::ivec3 position0;
    glm::ivec3 position1;
};

struct Structure {
    StructureId type;
    int variation;
    std::vector<Section> sections;
    
};
