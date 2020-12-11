#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "TreeNoise.hpp"

struct GenerationData;
class WorldGenerator;
struct Section;

class StructureGenerator {
private:
    WorldGenerator* m_worldGenerator;
    TreeNoise m_treeNoise;    

    static void fillSection(const Section& section, const glm::ivec3& position, GenerationData* data);
    std::vector<glm::ivec3> getTreePositions(const glm::ivec2& chunk) const;

    void generateTrees(const glm::ivec2 &chunk, GenerationData *data) const;    
public:
    StructureGenerator(WorldGenerator *worldGenerator);

    void generateStructures(const glm::vec2 &chunk, GenerationData* data) const;
};
