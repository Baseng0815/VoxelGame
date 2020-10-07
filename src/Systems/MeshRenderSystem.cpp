#include "../../include/Systems/MeshRenderSystem.hpp"

#include "../../include/Resources/ResourceManager.hpp"
#include "../../include/Resources/Texture.hpp"

#include "../../include/Events/Event.hpp"
#include "../../include/Events/EventDispatcher.hpp"

#include "../../include/Components/CameraComponent.hpp"
#include "../../include/Components/ChunkComponent.hpp"
#include "../../include/Components/MeshRenderComponent.hpp"
#include "../../include/Components/MultiMeshRenderComponent.hpp"
#include "../../include/Components/PlayerComponent.hpp"
#include "../../include/Components/TransformationComponent.hpp"

void MeshRenderSystem::handleFramebufferSize(const FramebufferSizeEvent &e) {
    m_waterRenderbuffers->resize(e.width, e.height);
}

void MeshRenderSystem::uploadToShader(const Shader *shader, const CameraComponent &camera) const {
    shader->upload("viewMatrix", camera.viewMatrix);
    shader->upload("viewPos", glm::vec3{1.f, 0.f, 0.f});
    shader->upload("projectionMatrix", camera.perspectiveProjection);

    for (int i = 0; i < MAX_LIGHTS; i++) {
        shader->upload("pointLights[" + std::to_string(i) + "]", m_pointLights[i]);
    }

    shader->upload("dirLight", m_sun);
    shader->setUniformState(true);
}

void MeshRenderSystem::render(const TransformationComponent &transformation, const MeshRenderComponent &meshRenderer, const CameraComponent &camera) const {
    if (meshRenderer.geometry->getDrawCount() > 0) {
        const Shader *shader = meshRenderer.material->customShader;
        // no custom shader is used
        if (!shader) {
            shader = meshRenderer.material->diffuseMap ? m_meshRenderShaderTexture : m_meshRenderShaderColor;
            shader->bind();
            shader->upload("modelMatrix", transformation.getModelMatrix());
            shader->upload("material", *meshRenderer.material);

            // bind texture if diffuse map is specified
            if (meshRenderer.material->diffuseMap) {
                meshRenderer.material->diffuseMap->bind(GL_TEXTURE0);
                // use given specular map
                if (meshRenderer.material->specularMap) {
                    meshRenderer.material->specularMap->bind(GL_TEXTURE1);
                    // no specular map given, use full-white texture
                }
                else {
                    ResourceManager::getResource<Texture>(TEXTURE_WHITE)->bind(GL_TEXTURE1);
                }
            }
            // use custom shader
        }
        else {
            shader->bind();
            if (!shader->uniformsSet()) {
                uploadToShader(shader, camera);
                shader->setUniformState(true);
            }
            shader->upload("modelMatrix", transformation.getModelMatrix());
            shader->upload("material", *meshRenderer.material);
        }

        // some more opengl states
        if (meshRenderer.material->useBlending) {
            glEnable(GL_BLEND);
        }
        else {
            glDisable(GL_BLEND);
        }

        if (meshRenderer.material->useCulling) {
            glEnable(GL_CULL_FACE);
        }
        else {
            glDisable(GL_CULL_FACE);
        }

        if(meshRenderer.water) {
            m_waterRenderbuffers->bind(RenderbufferId::RENDERBUFFER_WATER);            
        }
        else {
            m_waterRenderbuffers->bind(RenderbufferId::RENDERBUFFER_SCENE);            
        }

        // final draw call
        glBindVertexArray(meshRenderer.geometry->getVao());
        glDrawElements(GL_TRIANGLES, meshRenderer.geometry->getDrawCount(), GL_UNSIGNED_INT, nullptr);
    }
}

void MeshRenderSystem::updateFramebuffer() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glEnable(GL_BLEND);

    m_framebufferShader->bind();
    m_waterRenderbuffers->bindTextures();
    m_screenRenderquad.render();    
}

void MeshRenderSystem::_update(int dt) {
    m_waterRenderbuffers->clear();

    // upload per-frame values for both shaders
    // color shader
    m_meshRenderShaderColor->bind();
    const CameraComponent &camera = m_registry.view<CameraComponent>().raw()[0];    

    // texture shader
    m_meshRenderShaderTexture->bind();
    uploadToShader(m_meshRenderShaderTexture, camera);

    // color shader
    m_meshRenderShaderColor->bind();
    uploadToShader(m_meshRenderShaderColor, camera);
    
    // render single mesh components
    m_registry.view<TransformationComponent, MeshRenderComponent>().each(
        [&](const TransformationComponent &transformation, const MeshRenderComponent &meshRenderer) {
            render(transformation, meshRenderer, camera);
        });

    // render multi mesh components
    m_registry.view<TransformationComponent, MultiMeshRenderComponent>().each(
        [&](const TransformationComponent &transformation, const MultiMeshRenderComponent &multiMeshRenderer) {
            for (const auto &meshRenderer : multiMeshRenderer.meshRenderComponents) {
                render(transformation, meshRenderer, camera);
            }
        });   

    updateFramebuffer();
}

MeshRenderSystem::MeshRenderSystem(Registry_T &registry)
    : System{registry, 0},
      m_meshRenderShaderColor{ResourceManager::getResource<Shader>(SHADER_MESH_RENDER_COLOR)},
      m_meshRenderShaderTexture{ResourceManager::getResource<Shader>(SHADER_MESH_RENDER_TEXTURE)},
      m_waterRenderbuffers{new WaterRenderbuffers{800, 600}},
      m_screenRenderquad{Rectangle{-1, -1, 2, 2}},
      m_framebufferShader{ResourceManager::getResource<Shader>(SHADER_FRAMEBUFFER)} {
    m_sun.direction = glm::vec3{-0.3, -0.8, -0.5};
    m_sun.ambient = glm::vec3{0.2f};
    m_sun.diffuse = glm::vec3{0.5f};
    m_sun.specular = glm::vec3{0.2f};

    m_framebufferCallbackHandle = EventDispatcher::onFramebufferSize.subscribe(
        [&](const FramebufferSizeEvent &e) {
            handleFramebufferSize(e);
        });
}
