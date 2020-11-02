#pragma once

#include <glm/glm.hpp>

#include "Resource.hpp"
#include "../Color.hpp"

class Texture;
class Shader;

class Material : public Resource {
    protected:
        void release() override;

    public:
        // texture properties
        const Texture *diffuseMap   = nullptr;
        const Texture *specularMap  = nullptr;

        // color properties
        Color color;
        float shininess = 2.f;
        const Shader *customShader = nullptr;
        bool useBlending    = false;
        bool useCulling     = true;
};
