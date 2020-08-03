#pragma once

#include <map>
#include <vector>
#include <memory>
#include <string>

class Resource;
using ResourceHandle = std::unique_ptr<Resource>;

class ResourceManager {
    private:

        // TODO RAII this whole thing
        static std::map<std::string, ResourceHandle> resources;

    public:
        static void loadResources();

        static const Resource* getResourceBase(const std::string& id);
        template <class T>
        static const T* getResource(const std::string& id);
};
