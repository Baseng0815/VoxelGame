#pragma once

#include <string>
#include <GL/glew.h>
#include <vector>
#include "Vertex.h"

class Geometry {
    private:
        GLuint m_vao, m_vbo, m_ebo;
        unsigned int m_drawCount;

        void initBuffers();

    public:
        Geometry(const std::string& file);
        Geometry(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
        Geometry();

        void fillBuffers(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

        GLuint getVao() const;
        GLuint getVbo() const;
        GLuint getEbo() const;
        unsigned int getDrawCount() const;
};
