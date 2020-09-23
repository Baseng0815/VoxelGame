#include "../../include/Resources/TextureAtlas.hpp"

#include "../../include/GameData/BlockTemplate.hpp"
#include "../../include/GameData/GameData.hpp"
#include "../../include/Resources/Texture.hpp"

#include <vector>

TextureAtlas::TextureAtlas(int rows, int cols)
    : rows(rows), cols(cols) {
    uvXpT = 1 / (float)cols;
    uvYpT = 1 / (float)rows;

    // generate face uvs
    std::vector<FaceUVs> faceUVs;
    for (int i = 0; i < cols * rows; i++) {
        glm::vec2 topLeft = glm::vec2{i % cols / (float)cols, i / cols / (float)rows};
        faceUVs.push_back({topLeft,
                           glm::vec2{topLeft.x + uvXpT, topLeft.y + uvYpT},
                           glm::vec2{topLeft.x + uvXpT, topLeft.y},
                           glm::vec2{topLeft.x, topLeft.y + uvYpT}});
    }

    // generate block uv arrays
    for (size_t i = 0; i < (size_t)BlockId::NUM_BLOCKS; i++) {
        const BlockTemplate &blockTemplate = GameData::getBlockTemplate((BlockId)i);
        blockUVsArray[i][0] = faceUVs[blockTemplate.tid_py];
        blockUVsArray[i][1] = faceUVs[blockTemplate.tid_nx];
        blockUVsArray[i][2] = faceUVs[blockTemplate.tid_px];
        blockUVsArray[i][3] = faceUVs[blockTemplate.tid_nz];
        blockUVsArray[i][4] = faceUVs[blockTemplate.tid_pz];
        blockUVsArray[i][5] = faceUVs[blockTemplate.tid_ny];
    }
}

const BlockUVs &TextureAtlas::operator[](const BlockId &id) const {
    return blockUVsArray[(size_t)id];
}
