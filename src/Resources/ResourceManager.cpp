#include "../../include/Resources/ResourceManager.hpp"

#include <iostream>

#include "../../include/Resources/Font.hpp"
#include "../../include/Resources/Shader.hpp"
#include "../../include/Resources/Texture.hpp"
#include "../../include/Resources/Geometry.hpp"
#include "../../include/Resources/Material.hpp"

std::map<ResourceID, ResourceHandle> ResourceManager::resources;

void ResourceManager::loadResources() {
    // using a temporary because if allocation fails, the raw pointer would dangle
    // with a temporary, if allocation fails, memory is automatically cleaned up

    // textures
    resources.emplace(TEXTURE_ATLAS, ResourceHandle {new Texture("Textures/textureAtlas0.png")});
    resources.emplace(TEXTURE_BACKGROUND_MAIN_MENU, ResourceHandle {new Texture("Textures/background_mainmenu.png")});
    resources.emplace(TEXTURE_WHITE, ResourceHandle {new Texture("Textures/white.png")});
    resources.emplace(TEXTURE_BLACK, ResourceHandle {new Texture("Textures/black.png")});
    resources.emplace(TEXTURE_CROSSHAIR, ResourceHandle {new Texture("Textures/crosshair.png")});
    resources.emplace(TEXTURE_CUBE_SKYBOX, ResourceHandle {new Texture({
        "Textures/Skybox/1_0.png", "Textures/Skybox/1_1.png", "Textures/Skybox/1_2.png", "Textures/Skybox/1_3.png", "Textures/Skybox/1_4.png", "Textures/Skybox/1_5.png",
    })});

    // geometries
    resources.emplace(GEOMETRY_GRASS, ResourceHandle {new Geometry("Models/grass.obj")});

    // shaders
    // render 3d meshes with lighting and texturing
    resources.emplace(SHADER_MESH_RENDER_TEXTURE, ResourceHandle {new Shader("Shaders/meshRenderShader.vert", "Shaders/meshRenderShaderTexture.frag")});
    // render 3d meshes with lighting and color
    resources.emplace(SHADER_MESH_RENDER_COLOR, ResourceHandle {new Shader("Shaders/meshRenderShader.vert", "Shaders/meshRenderShaderColor.frag")});
    // render 2d quads using texture
    resources.emplace(SHADER_TEXTURE_QUAD, ResourceHandle {new Shader("Shaders/texturedQuadShader")});
    // render 2d quads using color
    resources.emplace(SHADER_COLOR_QUAD, ResourceHandle {new Shader("Shaders/coloredQuadShader")});
    // render 3d skybox
    resources.emplace(SHADER_SKYBOX, ResourceHandle {new Shader("Shaders/skyboxShader")});
    // render 2d text
    resources.emplace(SHADER_TEXT, ResourceHandle {new Shader("Shaders/textShader")});
    // render 3d color
    resources.emplace(SHADER_MVP_COLOR, ResourceHandle {new Shader("Shaders/mvpColorShader")});

    // materials
    Material *material = new Material {};
    material->diffuseMap = ResourceManager::getResource<Texture>(TEXTURE_ATLAS);
    material->specularMap = ResourceManager::getResource<Texture>(TEXTURE_BLACK);
    material->shininess = 32.f;
    resources.emplace(MATERIAL_CHUNK_BLOCKS_CULLED, ResourceHandle {material});

    material = new Material {};
    material->diffuseMap = ResourceManager::getResource<Texture>(TEXTURE_ATLAS);
    material->specularMap = ResourceManager::getResource<Texture>(TEXTURE_BLACK);
    material->shininess = 32.f;
    material->useCulling = false;
    resources.emplace(MATERIAL_CHUNK_BLOCKS_NON_CULLED, ResourceHandle {material});

    material = new Material {};
    material->useCulling = false;
    resources.emplace(MATERIAL_CLOUDS, ResourceHandle {material});

    material = new Material {};
    material->color = Color::Green;
    resources.emplace(MATERIAL_GRASS, ResourceHandle {material});

    // fonts
    resources.emplace(FONT_KORURI, ResourceHandle {new Font("Fonts/Koruri-Regular.ttf")});

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

    // skybox shader
    shader = getResource<Shader>(SHADER_SKYBOX);
    shader->setAttributes({"vertex"});

    // mvp color shader
    shader = getResource<Shader>(SHADER_MVP_COLOR);
    shader->setAttributes({"position"});

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
