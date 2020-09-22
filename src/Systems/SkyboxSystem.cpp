#include "../../include/Systems/SkyboxSystem.hpp"

#include "../../include/Resources/ResourceManager.hpp"
#include "../../include/Resources/Shader.hpp"

#include "../../include/Components/SkyboxComponent.hpp"
#include "../../include/Components/CameraComponent.hpp"

void SkyboxSystem::_update(int dt)
{
    const SkyboxComponent &skybox = m_registry.view<SkyboxComponent>().raw()[0];
    const CameraComponent &camera = m_registry.view<CameraComponent>().raw()[0];

    m_skyboxShader->bind();
    if (!m_skyboxShader->uniformsSet()) {
        m_skyboxShader->upload("viewMatrix", camera.viewMatrix);
        m_skyboxShader->upload("projectionMatrix", camera.perspectiveProjection);
        m_skyboxShader->setUniformState(true);
    }

    skybox.texture->bind(GL_TEXTURE0, GL_TEXTURE_CUBE_MAP);

    glDisable(GL_CULL_FACE);
    glBindVertexArray(skybox.geometry.getVao());
    glDrawElements(GL_TRIANGLES, skybox.geometry.getDrawCount(), GL_UNSIGNED_INT, nullptr);
    glEnable(GL_CULL_FACE);
}

SkyboxSystem::SkyboxSystem(Registry_T &registry)
    : System {registry, 0}, m_skyboxShader {ResourceManager::getResource<Shader>(SHADER_SKYBOX)}
{
    const constexpr int &SIZE = Configuration::SKYBOX_SIZE;

    const std::vector<Vertex> vertices = {
        Vertex {glm::vec3 {-SIZE, SIZE, -SIZE}},
        Vertex {glm::vec3 {-SIZE, -SIZE, SIZE}},
        Vertex {glm::vec3 {-SIZE, SIZE, SIZE}},
        Vertex {glm::vec3 {-SIZE, -SIZE, -SIZE}},
        Vertex {glm::vec3 {SIZE, SIZE, SIZE}},
        Vertex {glm::vec3 {SIZE, -SIZE, -SIZE}},
        Vertex {glm::vec3 {SIZE, SIZE, -SIZE}},
        Vertex {glm::vec3 {SIZE, -SIZE, SIZE}}
    };

    const std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 3, 1,
        0, 4, 6,
        0, 2, 4,
        4, 5, 6,
        4, 7, 5,
        2, 7, 4,
        2, 1, 7,
        6, 3, 0,
        6, 5, 3,
        1, 5, 7,
        1, 3, 5
    };

    Geometry skyboxGeometry {vertices, indices};

    auto skyboxEntity = registry.create();
    registry.emplace<SkyboxComponent>(skyboxEntity, SkyboxComponent { std::move(skyboxGeometry),
        ResourceManager::getResource<Texture>(TEXTURE_CUBE_SKYBOX) });
}
