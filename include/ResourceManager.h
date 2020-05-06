#pragma once

#include <map>
#include <vector>
#include <memory>
#include <string>

class Resource;

class ResourceManager {
    private:
        static std::map<std::string, Resource*> resources;

    public:
        static void loadResources();
        static void freeResources();

        static Resource* getResourceBase(const std::string& id);
        template <class T>
        static T* getResource(const std::string& id);
};
