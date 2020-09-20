#include "BlockTemplate.h"

#include <vector>

// this class holds game data:
// -> block templates (side textures, transparency etc.)
// -> item templates (textures, properties etc.)
class GameData {
    private:
        static std::vector<BlockTemplate> m_blockTemplates;

    public:
        static void loadGameData();

        static const BlockTemplate &getBlockTemplate(BlockId block);
};
