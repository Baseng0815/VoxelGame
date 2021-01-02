#include "../../include/Systems/DebugRenderSystem.hpp"

#include "../../include/Color.hpp"
#include "../../include/Resources/Shader.hpp"
#include "../../include/Events/EventDispatcher.hpp"
#include "../../include/Resources/ResourceManager.hpp"

#include "../../include/Components/CameraComponent.hpp"
#include "../../include/Components/TransformationComponent.hpp"

#include <glm/gtx/transform.hpp>

void DebugRenderSystem::recalculateCoordinateSystemModelMatrix()
{
    const CameraComponent           &camera     = m_registry.get<CameraComponent>(m_cameraEntity);
    const TransformationComponent   &transform  = m_registry.get<TransformationComponent>(m_cameraEntity);

    glm::vec3 cameraPositionAbsolute = transform.getPosition() + camera.positionOffset;
    cameraPositionAbsolute += 3.f * camera.front;

    m_coordinateSystemModelMatrix = glm::translate(cameraPositionAbsolute);
}

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

        const CameraComponent &camera = m_registry.get<CameraComponent>(m_cameraEntity);
        m_mvpColorShader->upload("viewMatrix", camera.viewMatrix);
        m_mvpColorShader->upload("projectionMatrix", camera.perspectiveProjection);

        m_mvpColorShader->upload("modelMatrix", m_chunkModelMatrix);
        m_mvpColorShader->upload("material.color", Color::Red);

        // draw chunk boundaries
        glBindVertexArray(m_chunkBoundaries.getVao());
        glDrawElements(GL_LINES, m_chunkBoundaries.getDrawCount(), GL_UNSIGNED_INT, nullptr);

        m_mvpColorShader->upload("modelMatrix", m_coordinateSystemModelMatrix);
        m_mvpColorShader->upload("material.color", Color::Blue);

        // draw coordinate system
        glDisable(GL_DEPTH_TEST);
        glBindVertexArray(m_coordinateSystem.getVao());
        glDrawElements(GL_LINES, m_coordinateSystem.getDrawCount(), GL_UNSIGNED_INT, nullptr);
        glEnable(GL_DEPTH_TEST);
    }
}

DebugRenderSystem::DebugRenderSystem(Registry_T &registry)
    : System {registry}, m_mvpColorShader {ResourceManager::getResource<Shader>(SHADER_MVP_COLOR)},
    m_cameraEntity {m_registry.view<CameraComponent>().front()}
{
    // create chunk boundaries
    std::vector<Vertex> vertices;
    vertices.reserve((Configuration::CHUNK_COUNT_PER_AXIS + 1) * 4 * 2);

    std::vector<unsigned int> indices;
    indices.reserve((Configuration::CHUNK_COUNT_PER_AXIS + 1) * 6 * 2);

    int counter = 0;
    for (int x = -Configuration::CHUNK_PRELOAD_SIZE; x <= (Configuration::CHUNK_PRELOAD_SIZE + 1); x++) {
        for (int z = -Configuration::CHUNK_PRELOAD_SIZE; z <= (Configuration::CHUNK_PRELOAD_SIZE + 1); z++) {
            const float p_x = x * Configuration::CHUNK_SIZE;
            const float p_z = z * Configuration::CHUNK_SIZE;

            vertices.emplace_back(Vertex {glm::vec3 {p_x, 0.f, p_z}});
            vertices.emplace_back(Vertex {glm::vec3 {p_x, Configuration::CHUNK_HEIGHT, p_z}});
            indices.emplace_back(counter++);
            indices.emplace_back(counter++);
        }
    }

    m_chunkBoundaries.fillBuffers(vertices, indices);

    // create coordinate system
    vertices = {
        Vertex {glm::vec3 {0.f, 0.f, 0.f}},
        Vertex {glm::vec3 {1.f, 0.f, 0.f}},
        Vertex {glm::vec3 {0.f, 1.f, 0.f}},
        Vertex {glm::vec3 {0.f, 0.f, 1.f}}
    };

    indices = {0, 1, 0, 2, 0, 3};
    m_coordinateSystem.fillBuffers(vertices, indices);

    m_keyEventHandle = EventDispatcher::onKeyPress.subscribe([&](const KeyEvent &e) {
        handleKeys(e);
    });

    m_enterChunkHandle = EventDispatcher::onEnterChunk.subscribe([&](const EnterChunkEvent &e) {
        handleEnterChunk(e);
    });

    m_cursorHandle = EventDispatcher::onCursorMove.subscribe([&](const CursorEvent&) {
        recalculateCoordinateSystemModelMatrix();
    });

    m_entityMovedHandle = EventDispatcher::onEntityMoved.subscribe([&](const EntityMovedEvent&) {
        recalculateCoordinateSystemModelMatrix();
    });
}
