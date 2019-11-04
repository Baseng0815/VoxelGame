#pragma once

#include "pch.h"

#include "Mesh.h"
#include "Texture.h"

#include "ResourceDefinitions.h"

class ResourceRegistry {
private:
	// map each resource to an ID
	static std::map<short, Texture> m_loadedTextures;
	static std::map<short, Mesh> m_loadedMeshes;

	static void loadModels();
	static void loadTextures();

public:

	static void loadResources();
	static void freeResources();
};
