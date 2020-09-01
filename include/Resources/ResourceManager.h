#pragma once

#include <map>
#include <vector>
#include <memory>
#include <string>

#include "ResourceIDs.h"

class Resource;
using ResourceHandle = std::unique_ptr<Resource>;

class ResourceManager {
    private:

        // TODO RAII this whole thing
        static std::map<ResourceID, ResourceHandle> resources;

    public:
        static void loadResources();

        static const Resource* getResourceBase(const ResourceID &id);
        template <class T>
        static const T* getResource(const ResourceID& id);
};
