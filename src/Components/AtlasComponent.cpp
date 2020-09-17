#include "../../include/Components/AtlasComponent.h"

#include <iostream>

AtlasComponent::AtlasComponent(int width, int height, int tileSize) {
    numCols = width / tileSize;
    numRows = height / tileSize;
    uvXpT = 1 / (float)numCols;
    uvYpT = 1 / (float)numRows;

    std::vector<FaceUVs> faceUVs;
    // generate face uvs
    for (int i = 0; i < numCols * numRows; i++) {
        glm::vec2 topLeft = glm::vec2(i % numCols / (float)numCols , i / numCols / (float)numRows);
        faceUVs.push_back({ topLeft, glm::vec2(topLeft.x + uvXpT, topLeft.y + uvYpT),
                glm::vec2(topLeft.x + uvXpT, topLeft.y), glm::vec2(topLeft.x, topLeft.y + uvYpT)});
    }

    // generate block uv arrays
    for (int i = 0; i < blockUVsArray.size(); i++) {
        auto& blockData = Configuration::getBlockData((BlockType)i);
        blockUVsArray[i][0] = faceUVs[blockData.tid_up];
        blockUVsArray[i][1] = faceUVs[blockData.tid_nx];
        blockUVsArray[i][2] = faceUVs[blockData.tid_px];
        blockUVsArray[i][3] = faceUVs[blockData.tid_nz];
        blockUVsArray[i][4] = faceUVs[blockData.tid_pz];
        blockUVsArray[i][5] = faceUVs[blockData.tid_bot];
    }
}
