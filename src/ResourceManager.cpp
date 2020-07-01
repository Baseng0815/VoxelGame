#include "../include/ResourceManager.h"
#include <utility>

#include "../include/Rendering/Font.h"
#include "../include/Rendering/Shader.h"
#include "../include/Rendering/Texture.h"

std::map<std::string, void*> ResourceManager::resources;

// TODO remove
#include <iostream>

void ResourceManager::loadResources() {
    // textures
    resources.emplace("textureAtlas", new Texture("Textures/textureAtlas0.png"));
    resources.emplace("textureBackgroundMainMenu", new Texture("Textures/background_mainmenu.png"));

    // shaders
    resources.emplace("shaderMeshRender", new Shader("Shaders/meshRenderShader"));
    resources.emplace("shaderTexturedQuad", new Shader("Shaders/texturedQuadShader"));
    resources.emplace("shaderColoredQuad", new Shader("Shaders/coloredQuadShader"));
    resources.emplace("skyboxShader", new Shader("Shaders/skyboxShader"));
    resources.emplace("shaderText", new Shader("Shaders/textShader"));

    // meshRenderShader
    Shader* shader = getResource<Shader>("shaderMeshRender");
    shader->setAttributes({"position", "normal", "uvCoords"});

    // quad shaders
    shader = getResource<Shader>("shaderTexturedQuad");
    shader->setAttributes({"vertex"});
    shader = getResource<Shader>("shaderColoredQuad");
    shader->setAttributes({"vertex"});

    // fonts
    resources.emplace("fontKoruri", new Font("Fonts/Koruri-Regular.ttf"));
}

void ResourceManager::freeResources() {
    for (auto resource : resources)
        delete resource.second;
}

void* ResourceManager::getResourceBase(const std::string& id) {
    auto it = resources.find(id);
    if (it == resources.end())
        return nullptr;
    else return it->second;
}

template<class T>
T* ResourceManager::getResource(const std::string& id) {
    return reinterpret_cast<T*>(getResourceBase(id));
}

template Texture* ResourceManager::getResource<Texture>(const std::string&);
template Shader* ResourceManager::getResource<Shader>(const std::string&);
template Font* ResourceManager::getResource<Font>(const std::string&);
