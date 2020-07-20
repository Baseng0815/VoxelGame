#pragma once

#include <string>
#include <GL/glew.h>
#include <vector>
#include "../Rendering/Vertex.h"

#include "Resource.h"

class Geometry : public Resource {
    private:
        GLuint m_vao, m_vbo, m_ebo;
        unsigned int m_drawCount;
        bool m_isReady = false;

        void initBuffers();

    public:
        Geometry(const std::string& file);
        Geometry(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
        Geometry();
        void free() override;

        void fillBuffers(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

        GLuint getVao() const;
        GLuint getVbo() const;
        GLuint getEbo() const;
        unsigned int getDrawCount() const;
        bool isReady() const;
};
