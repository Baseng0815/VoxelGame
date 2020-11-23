#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Structures.hpp"

#include "TreeNoise.hpp"

struct GenerationData;
class WorldGenerator;

class StructureGenerator {
private:
    WorldGenerator* m_worldGenerator;
    TreeNoise m_treeNoise;

    void generateStructure(const glm::vec2 &chunk, glm::vec3 position, const BlockCollection& structure, Block*** blocks) const;

    std::vector<glm::ivec2> getTreePositions(int chunkX, int chunkZ) const;

    void generateTrees(const glm::vec2 &chunk, GenerationData *data) const;    
public:
    StructureGenerator(WorldGenerator *worldGenerator);

    void generateStructures(const glm::vec2 &chunk, GenerationData* data) const;
};
