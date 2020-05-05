#pragma once

#include "Shader.h"

class BlockShader : public Shader {
    private:
        GLint m_locationModelMatrix;
        GLint m_locationProjectionMatrix;
        GLint m_locationViewMatrix;

    public:
        // load shader and get uniform locations
        BlockShader();

        void uploadModelMatrix(glm::mat4 modelMatrix) const;
        void uploadProjectionMatrix(glm::mat4 projectionMatrix) const;
        void uploadViewMatrix(glm::mat4 viewMatrix) const;
};
