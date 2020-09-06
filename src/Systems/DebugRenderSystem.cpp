#include "../../include/Systems/DebugRenderSystem.h"

#include "../../include/Configuration.h"
#include "../../include/Resources/Shader.h"
#include "../../include/Resources/Geometry.h"
#include "../../include/Resources/ResourceManager.h"

#include "../../include/Components/CameraComponent.h"

void DebugRenderSystem::_update(int dt)
{
    if (m_isEnabled) {
        m_mvpColorShader->bind();

        const CameraComponent &camera = m_registry.view<CameraComponent>().raw()[0];
        m_mvpColorShader->upload("viewMatrix", camera.viewMatrix);
        m_mvpColorShader->upload("projectionMatrix", camera.perspectiveProjection);

        m_mvpColorShader->upload("modelMatrix", glm::mat4 {1.f});
        m_mvpColorShader->upload("color", glm::vec4 {1.0f, 0.1f, 0.1f, 0.5f});

        // draw chunk boundaries
        glEnable(GL_BLEND);

        for (const auto &geometry : m_chunkBoundaries) {
            glBindVertexArray(geometry.getVao());
            glDrawElements(GL_TRIANGLES, geometry.getDrawCount(), GL_UNSIGNED_INT, nullptr);
        }

        glDisable(GL_BLEND);
    }
}

DebugRenderSystem::DebugRenderSystem(Registry_T &registry)
    : System {registry, 0}, m_mvpColorShader {ResourceManager::getResource<Shader>(SHADER_MVP_COLOR)}
{
    std::vector<Vertex> vertices;
    vertices.reserve((Configuration::CHUNK_COUNT_PER_AXIS + 1) * 4 * 2);

    std::vector<unsigned int> indices;
    indices.reserve((Configuration::CHUNK_COUNT_PER_AXIS + 1) * 6 * 2);

    float left = -Configuration::CHUNK_PRELOAD_SIZE * Configuration::CHUNK_SIZE;
    float right = Configuration::CHUNK_PRELOAD_SIZE * Configuration::CHUNK_SIZE;

    // along x-axis
    for (int x = {-Configuration::CHUNK_PRELOAD_SIZE}; x <= Configuration::CHUNK_PRELOAD_SIZE + 1; x++) {
        vertices.emplace_back(Vertex {glm::vec3 {left, Configuration::CHUNK_HEIGHT, x * Configuration::CHUNK_SIZE}});
        vertices.emplace_back(Vertex {glm::vec3 {right, Configuration::CHUNK_HEIGHT, x * Configuration::CHUNK_SIZE}});
        vertices.emplace_back(Vertex {glm::vec3 {right, 0, x * Configuration::CHUNK_SIZE}});
        vertices.emplace_back(Vertex {glm::vec3 {left, 0, x * Configuration::CHUNK_SIZE}});
    }
}

void DebugRenderSystem::toggle()
{
    m_isEnabled = !m_isEnabled;
}
