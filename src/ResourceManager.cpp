#include "../include/ResourceManager.h"
//#include <utility>
#include <iostream>

#include "../include/Resources/Font.h"
#include "../include/Resources/Shader.h"
#include "../include/Resources/Texture.h"
#include "../include/Resources/Geometry.h"
#include "../include/Resources/Material.h"

std::map<std::string, ResourceHandle> ResourceManager::resources;

void ResourceManager::loadResources() {
    // textures
    resources.emplace("textureAtlas", std::unique_ptr<Resource>(new Texture("Textures/textureAtlas0.png")));
    resources.emplace("textureBackgroundMainMenu", std::unique_ptr<Resource>(new Texture("Textures/background_mainmenu.png")));
    resources.emplace("textureWhite", std::unique_ptr<Resource>(new Texture("Textures/white.png")));
    resources.emplace("textureBlack", std::unique_ptr<Resource>(new Texture("Textures/black.png")));

    // shaders
    resources.emplace("shaderMeshRenderTexture", std::unique_ptr<Resource>(new Shader("Shaders/meshRenderShader.vert", "Shaders/meshRenderShaderTexture.frag")));
    resources.emplace("shaderMeshRenderColor", std::unique_ptr<Resource>(new Shader("Shaders/meshRenderShader.vert", "Shaders/meshRenderShaderColor.frag")));
    resources.emplace("shaderTexturedQuad", std::unique_ptr<Resource>(new Shader("Shaders/texturedQuadShader")));
    resources.emplace("shaderColoredQuad", std::unique_ptr<Resource>(new Shader("Shaders/coloredQuadShader")));
    resources.emplace("skyboxShader", std::unique_ptr<Resource>(new Shader("Shaders/skyboxShader")));
    resources.emplace("shaderText", std::unique_ptr<Resource>(new Shader("Shaders/textShader")));

    // materials
    resources.emplace("materialChunkBlocks", std::unique_ptr<Resource>(new Material {ResourceManager::getResource<Texture>("textureAtlas"),
                ResourceManager::getResource<Texture>("textureBlack"), glm::vec3 {0.0f}, glm::vec3 {0.0f}, glm::vec3 {0.0f}, 32.0f}));

    // meshRenderShaderTexture
    const Shader *shader = getResource<Shader>("shaderMeshRenderTexture");
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

const Resource* ResourceManager::getResourceBase(const std::string& id) {
    auto it = resources.find(id);
    if (it == resources.end())
        return nullptr;
    else return it->second.get();
}

template<class T>
const T* ResourceManager::getResource(const std::string& id) {
    return static_cast<const T*>(getResourceBase(id));
}

template const Texture* ResourceManager::getResource<Texture>(const std::string&);
template const Shader* ResourceManager::getResource<Shader>(const std::string&);
template const Font* ResourceManager::getResource<Font>(const std::string&);
template const Geometry* ResourceManager::getResource<Geometry>(const std::string&);
template const Material* ResourceManager::getResource<Material>(const std::string&);
