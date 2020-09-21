#pragma once

#include "../GameData/Block.h"
#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>

enum class StructureType : int { STRUCTURE_TREE };

struct StructureId {
    StructureType type;
    int variation;

    bool operator==(const StructureId& other) const;
};

class StructureIdHash {
    public:
        long operator()(const StructureId& id) const;
};

typedef std::vector<std::pair<glm::vec3, BlockId>> BlockCollection;

class Structures {
    private:
        static BlockCollection getTreeBlocks(int height);
        static std::unordered_map<StructureId, BlockCollection, StructureIdHash> structures;
        static bool initialized;

    public:
        static void init();	

        static const BlockCollection& getStructure(StructureType type, int variation);
};
