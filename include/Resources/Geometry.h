#pragma once

#include <string>
#include <GL/glew.h>
#include <vector>
#include "../Rendering/Vertex.h"

#include "Resource.h"

class Geometry : public Resource {
    private:
        GLuint m_vao, m_vbo, m_ebo;
        unsigned int m_drawCount = 0;

        void initBuffers();
        void release() override;

    public:
        Geometry(const std::string& file);
        Geometry(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
        Geometry();
        ~Geometry();

        Geometry(Geometry&&) noexcept;
        Geometry &operator=(Geometry&&);

        void fillBuffers(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

        GLuint getVao() const;
        GLuint getVbo() const;
        GLuint getEbo() const;
        unsigned int getDrawCount() const;
};
