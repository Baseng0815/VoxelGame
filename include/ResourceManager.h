#pragma once

#include <map>
#include <vector>
#include <memory>
#include <string>

class ResourceManager {
    private:
        static std::map<std::string, void*> resources;

    public:
        static void loadResources();
        static void freeResources();

        static void* getResourceBase(const std::string& id);
        template <class T>
        static T* getResource(const std::string& id);
};
