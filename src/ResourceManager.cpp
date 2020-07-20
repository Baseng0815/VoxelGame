#include "../include/ResourceManager.h"
//#include <utility>
#include <iostream>

#include "../include/Resources/Font.h"
#include "../include/Resources/Shader.h"
#include "../include/Resources/Texture.h"
#include "../include/Resources/Geometry.h"
#include "../include/Resources/Material.h"

std::map<std::string, Resource*> ResourceManager::resources;

void ResourceManager::loadResources() {
    // textures
    resources.emplace("textureAtlas", new Texture("Textures/textureAtlas0.png"));
    resources.emplace("textureBackgroundMainMenu", new Texture("Textures/background_mainmenu.png"));
    resources.emplace("textureWhite", new Texture("Textures/white.png"));
    resources.emplace("textureBlack", new Texture("Textures/black.png"));

    // shaders
    resources.emplace("shaderMeshRenderTexture", new Shader("Shaders/meshRenderShader.vert", "Shaders/meshRenderShaderTexture.frag"));
    resources.emplace("shaderMeshRenderColor", new Shader("Shaders/meshRenderShader.vert", "Shaders/meshRenderShaderColor.frag"));
    resources.emplace("shaderTexturedQuad", new Shader("Shaders/texturedQuadShader"));
    resources.emplace("shaderColoredQuad", new Shader("Shaders/coloredQuadShader"));
    resources.emplace("skyboxShader", new Shader("Shaders/skyboxShader"));
    resources.emplace("shaderText", new Shader("Shaders/textShader"));

    // materials
    resources.emplace("materialChunkBlocks", new Material(ResourceManager::getResource<Texture>("textureAtlas"),
                ResourceManager::getResource<Texture>("textureBlack"), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), 32.0f));

    // meshRenderShaderTexture
    Shader* shader = getResource<Shader>("shaderMeshRenderTexture");
    shader->setAttributes({"position", "normal", "uvCoords"});
    shader->upload("material.diffuseMap", 0);
    shader->upload("material.specularMap", 1);

    // meshRenderShaderColor
    shader = getResource<Shader>("shaderMeshRenderTexture");
    shader->setAttributes({"position", "normal", "uvCoords"});

    // quad shaders
    shader = getResource<Shader>("shaderTexturedQuad");
    shader->setAttributes({"vertex"});
    shader = getResource<Shader>("shaderColoredQuad");
    shader->setAttributes({"vertex"});

    // fonts
    resources.emplace("fontKoruri", new Font("Fonts/Koruri-Regular.ttf"));

    std::cout << "loaded " << resources.size() << " resources" << std::endl;
}

void ResourceManager::freeResources() {
    for (auto resource : resources) {
        resource.second->free();
        delete resource.second;
    }
}

Resource* ResourceManager::getResourceBase(const std::string& id) {
    auto it = resources.find(id);
    if (it == resources.end())
        return nullptr;
    else return &(*it->second);
}

template<class T>
T* ResourceManager::getResource(const std::string& id) {
    return reinterpret_cast<T*>(getResourceBase(id));
}

template Texture* ResourceManager::getResource<Texture>(const std::string&);
template Shader* ResourceManager::getResource<Shader>(const std::string&);
template Font* ResourceManager::getResource<Font>(const std::string&);
template Geometry* ResourceManager::getResource<Geometry>(const std::string&);
template Material* ResourceManager::getResource<Material>(const std::string&);
