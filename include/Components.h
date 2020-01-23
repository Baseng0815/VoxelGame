#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>

struct TransformationComponent {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    glm::mat4 getModelMatrix() const;
}

struct GeometryComponent {
    unsigned int drawCount;

    //bool useElementDrawing;
    GLuint vao, vbo, ebo;
}

struct ChunkComponent {
    Block*** blocks = nullptr;
    std::mutex blockMutex, vaoMutex;

    bool buffersInitialized = false;
    bool verticesOutdated   = false;
    bool buffersOutdated    = false;
    std::vector<std::future<void>> futures;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    std::atomic_bool threadActiveOnSelf;
    static std::atomic_int constructionCount;
}
