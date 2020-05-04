#include "../include/ResourceManager.h"
#include <utility>

#include "../include/Shader.h"
#include "../include/Texture.h"

std::map<std::string, Resource*> ResourceManager::resources;

void ResourceManager::loadResources() {
    // textures
    // TODO use Configuration::ResourceBasePath
    resources.insert(std::make_pair("textureAtlas", new Texture("Resources/Textures/textureAtlas0.png")));

    // shaders
    resources.insert(std::make_pair("chunkShader", new Shader("Resources/Shaders/chunkShader")));
    resources.insert(std::make_pair("guiShader", new Shader("Resources/Shaders/guiShader")));
    resources.insert(std::make_pair("skyboxShader", new Shader("Resources/Shaders/skyboxShader")));
    resources.insert(std::make_pair("lightingShader", new Shader("Resources/Shaders/lightingShader")));

    std::vector<std::string> attribs = {
        "position",
        "normal",
        "uvCoords"
    };

    // chunkShader
    Shader* shader = getResource<Shader>("chunkShader");
    shader->setAttributes(attribs);
    // lightingShader
    shader = getResource<Shader>("lightingShader");
    attribs = {
            "position",
            "uvCoords"
    };
    shader->setAttributes(attribs);
    shader->upload("gPosition", 0);
    shader->upload("gNormal", 1);
    shader->upload("gAlbedo", 2);
}

void ResourceManager::freeResources() {
    for (auto resource : resources)
        delete resource.second;
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
