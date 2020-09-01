#include "../../include/Resources/ResourceManager.h"

#include <iostream>

#include "../../include/Resources/Font.h"
#include "../../include/Resources/Shader.h"
#include "../../include/Resources/Texture.h"
#include "../../include/Resources/Geometry.h"
#include "../../include/Resources/Material.h"

std::map<ResourceID, ResourceHandle> ResourceManager::resources;

void ResourceManager::loadResources() {
    // textures
    resources.emplace(TEXTURE_ATLAS, std::unique_ptr<Resource> {new Texture("Textures/textureAtlas0.png")});
    resources.emplace(TEXTURE_BACKGROUND_MAIN_MENU, std::unique_ptr<Resource> {new Texture("Textures/background_mainmenu.png")});
    resources.emplace(TEXTURE_WHITE, std::unique_ptr<Resource> {new Texture("Textures/white.png")});
    resources.emplace(TEXTURE_BLACK, std::unique_ptr<Resource> {new Texture("Textures/black.png")});

    // shaders
    resources.emplace(SHADER_MESH_RENDER_TEXTURE, std::unique_ptr<Resource> {new Shader("Shaders/meshRenderShader.vert", "Shaders/meshRenderShaderTexture.frag")});
    resources.emplace(SHADER_MESH_RENDER_COLOR, std::unique_ptr<Resource> {new Shader("Shaders/meshRenderShader.vert", "Shaders/meshRenderShaderColor.frag")});
    resources.emplace(SHADER_TEXTURE_QUAD, std::unique_ptr<Resource> {new Shader("Shaders/texturedQuadShader")});
    resources.emplace(SHADER_COLOR_QUAD, std::unique_ptr<Resource> {new Shader("Shaders/coloredQuadShader")});
    resources.emplace(SHADER_SKYBOX, std::unique_ptr<Resource> {new Shader("Shaders/skyboxShader")});
    resources.emplace(SHADER_TEXT, std::unique_ptr<Resource> {new Shader("Shaders/textShader")});

    // materials
    resources.emplace(MATERIAL_CHUNK_BLOCKS, std::unique_ptr<Resource> {new Material {ResourceManager::getResource<Texture>(TEXTURE_ATLAS),
                ResourceManager::getResource<Texture>(TEXTURE_BLACK), glm::vec3 {0.0f}, glm::vec3 {0.0f}, glm::vec3 {0.0f}, 32.0f}});

    // fonts
    resources.emplace(FONT_KORURI, std::unique_ptr<Resource> {new Font("Fonts/Koruri-Regular.ttf")});

    // meshRenderShaderTexture
    const Shader *shader = getResource<Shader>(SHADER_MESH_RENDER_TEXTURE);
    shader->bind();
    shader->setAttributes({"position", "normal", "uvCoords"});
    shader->upload("material.diffuseMap", 0);
    shader->upload("material.specularMap", 1);

    // meshRenderShaderColor
    shader = getResource<Shader>(SHADER_MESH_RENDER_COLOR);
    shader->setAttributes({"position", "normal", "uvCoords"});

    // quad shaders
    shader = getResource<Shader>(SHADER_TEXTURE_QUAD);
    shader->setAttributes({"vertex"});
    shader = getResource<Shader>(SHADER_COLOR_QUAD);
    shader->setAttributes({"vertex"});

    std::cout << "loaded " << resources.size() << " resources" << std::endl;
}

const Resource* ResourceManager::getResourceBase(const ResourceID &id) {
    auto it = resources.find(id);
    if (it == resources.end())
        return nullptr;
    else return it->second.get();
}

template<class T>
const T* ResourceManager::getResource(const ResourceID &id) {
    return static_cast<const T*>(getResourceBase(id));
}

template const Texture* ResourceManager::getResource<Texture>(const ResourceID&);
template const Shader* ResourceManager::getResource<Shader>(const ResourceID&);
template const Font* ResourceManager::getResource<Font>(const ResourceID&);
template const Geometry* ResourceManager::getResource<Geometry>(const ResourceID&);
template const Material* ResourceManager::getResource<Material>(const ResourceID&);
