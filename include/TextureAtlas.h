#pragma once

#include "Definitions.h"

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

typedef std::array<glm::vec2, 4> FaceUVs;
typedef std::array<FaceUVs, 6> BlockUVs;
typedef std::array<BlockUVs, (int)BlockType::NUM_BLOCKS> BlockUVsArray;

// texture atlas used for block rendering
class TextureAtlas {
    private:
        GLuint m_texture;
        int m_width, m_height;
        int m_numRows, m_numCols;

        // uv per tile
        float m_uvXpT, m_uvYpT;

        // do all precalculations to save performance
        std::vector<FaceUVs> m_faceUVs;
        BlockUVsArray m_blockUVsArray;

    public:
        void init(const char* fileName, int tileSize = 16);
        ~TextureAtlas();

        // list of texture coordinates for a block type
        const BlockUVsArray* getBlockTextureCoordinates();

        void bind();
};
