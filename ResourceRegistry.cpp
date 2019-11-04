#include "pch.h"

#include "ResourceRegistry.h"

std::map<short, Texture> ResourceRegistry::m_loadedTextures;
std::map<short, Mesh> ResourceRegistry::m_loadedMeshes;

void ResourceRegistry::loadModels() {
	for (const auto& entry : _Resource_Meshes) {
		std::cout << entry << std::endl;

	}


}

void ResourceRegistry::loadTextures() {
	for (const auto &entry : _Resource_Textures) {
		std::cout << entry << std::endl;

	}

}

void ResourceRegistry::loadResources() {
	loadModels();
	loadTextures();
}

void ResourceRegistry::freeResources() {

}
