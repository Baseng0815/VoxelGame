#include "../../include/Systems/DebugRenderSystem.hpp"

#include "../../include/Color.hpp"
#include "../../include/Resources/Shader.hpp"
#include "../../include/Events/EventDispatcher.hpp"
#include "../../include/Resources/ResourceManager.hpp"

#include "../../include/Components/CameraComponent.hpp"

#include <glm/gtx/transform.hpp>

void DebugRenderSystem::handleKeys(const KeyEvent &e)
{
    if (e.action == GLFW_PRESS) {
        if (e.key == Configuration::getAssociatedKey("KEYBIND_TOGGLE_DEBUG")) {
            m_isEnabled = !m_isEnabled;
        }
    }
}

void DebugRenderSystem::handleEnterChunk(const EnterChunkEvent &e)
{
    m_chunkModelMatrix = glm::translate(glm::vec3 {e.newX * Configuration::CHUNK_SIZE, 0, e.newZ * Configuration::CHUNK_SIZE});
}

void DebugRenderSystem::_update(int dt)
{
    if (m_isEnabled) {
        m_mvpColorShader->bind();

        const CameraComponent &camera = m_registry.view<CameraComponent>().raw()[0];
        m_mvpColorShader->upload("viewMatrix", camera.viewMatrix);
        m_mvpColorShader->upload("projectionMatrix", camera.perspectiveProjection);

        m_mvpColorShader->upload("modelMatrix", m_chunkModelMatrix);
        m_mvpColorShader->upload("material.color", Color {255, 0, 0, 255});

        // draw chunk boundaries
        glBindVertexArray(m_chunkBoundaries.getVao());
        glDrawElements(GL_LINES, m_chunkBoundaries.getDrawCount(), GL_UNSIGNED_INT, nullptr);
    }
}

DebugRenderSystem::DebugRenderSystem(Registry_T &registry)
    : System {registry, 0}, m_mvpColorShader {ResourceManager::getResource<Shader>(SHADER_MVP_COLOR)}
{
    std::vector<Vertex> vertices;
    vertices.reserve((Configuration::CHUNK_COUNT_PER_AXIS + 1) * 4 * 2);

    std::vector<unsigned int> indices;
    indices.reserve((Configuration::CHUNK_COUNT_PER_AXIS + 1) * 6 * 2);

    int counter = 0;
    for (int x = -Configuration::CHUNK_PRELOAD_SIZE; x <= (Configuration::CHUNK_PRELOAD_SIZE + 1); x++) {
        for (int z = -Configuration::CHUNK_PRELOAD_SIZE; z <= (Configuration::CHUNK_PRELOAD_SIZE + 1); z++) {
            const float p_x = x * Configuration::CHUNK_SIZE;
            const float p_z = z * Configuration::CHUNK_SIZE;

            vertices.emplace_back(Vertex {glm::vec3 {p_x, 0, p_z}});
            vertices.emplace_back(Vertex {glm::vec3 {p_x, Configuration::CHUNK_HEIGHT, p_z}});
            indices.emplace_back(counter++);
            indices.emplace_back(counter++);
        }
    }

    m_chunkBoundaries.fillBuffers(vertices, indices);

    m_keyEventHandle = EventDispatcher::onKeyPress.subscribe([&](const KeyEvent &e) {
        handleKeys(e);
    });

    m_enterChunkHandle = EventDispatcher::onEnterChunk.subscribe([&](const EnterChunkEvent &e) {
        handleEnterChunk(e);
    });
}

void DebugRenderSystem::toggle()
{
    m_isEnabled = !m_isEnabled;
}
