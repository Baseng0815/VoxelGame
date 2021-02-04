#include "../../include/Resources/Geometry.hpp"

#include "../../include/Configuration.hpp"
#include "../../include/Utility/Utility.hpp"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <iostream>

void Geometry::initBuffers()
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // create empty buffers
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    // vao and attrib pointers
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(12));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(24));
}

void Geometry::release()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
    glDeleteVertexArrays(1, &m_vao);
}

Geometry::Geometry(const std::string& file)
{
    std::cout << "loading geometry " << file << "\n";
    initBuffers();

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(Configuration::getStringValue("ResourceBasePath") + file, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

    if (scene == nullptr|| !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "error: " << importer.GetErrorString() << "\n";
    }

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // only using one mesh without child or materials
    const aiMesh *mesh = scene->mMeshes[0];
    for (size_t i = 0; i < mesh->mNumVertices; i++) {
        glm::vec2 uv {0.f};
        if (mesh->mTextureCoords[0]) {
            uv.x = mesh->mTextureCoords[0][i].x;
            uv.y = mesh->mTextureCoords[0][i].y;
        }

        vertices.emplace_back(Vertex {glm::vec3 {
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        }, glm::vec3 {
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z
        }, uv});
    }

    for (size_t i = 0; i < mesh->mNumFaces; i++) {
        const aiFace &face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; j++) {
            indices.emplace_back(face.mIndices[j]);
        }
    }

    fillBuffers(vertices, indices);
}

Geometry::Geometry(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
    initBuffers();
    fillBuffers(vertices, indices);
}

Geometry::Geometry()
{
    initBuffers();
}

Geometry::~Geometry()
{
    release();
}

Geometry::Geometry(Geometry &&other) noexcept
    : m_vao(other.m_vao), m_vbo(other.m_vbo), m_ebo(other.m_ebo), m_drawCount(other.m_drawCount)
{
    other.m_vao = 0;
    other.m_vbo = 0;
    other.m_ebo = 0;
    other.m_drawCount = 0;
}

Geometry &Geometry::operator=(Geometry &&other)
{
    if (this != &other) {
        release();
        std::swap(m_vao, other.m_vao);
        std::swap(m_vbo, other.m_vbo);
        std::swap(m_ebo, other.m_ebo);
        std::swap(m_drawCount, other.m_drawCount);
    }

    return *this;
}

void Geometry::fillBuffers(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
    glNamedBufferData(m_vbo, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glNamedBufferData(m_ebo, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);

    m_drawCount = indices.size();
}

GLuint Geometry::getVao() const
{
    return m_vao;
}

GLuint Geometry::getVbo() const
{
    return m_vbo;
}

GLuint Geometry::getEbo() const
{
    return m_ebo;
}

unsigned int Geometry::getDrawCount() const
{
    return m_drawCount;
}
