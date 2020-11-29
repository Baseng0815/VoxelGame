#include "BlockTemplate.hpp"
#include "Structure.hpp"

#include <vector>

// this class holds game data:
// -> block templates (side textures, transparency etc.)
// -> item templates (textures, properties etc.)
class GameData {
  private:
    static std::vector<BlockTemplate> m_blockTemplates;
    static std::vector<Structure> m_structures;

  public:
    static void loadGameData();

    static const BlockTemplate& getBlockTemplate(BlockId block);
    static const Structure& getStructure(StructureId structure, int variation = 0);
};
