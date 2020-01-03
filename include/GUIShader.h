#pragma once

#include "Shader.h"
#include "Colors.h"

class GUIShader : public Shader {
    private:
        GLuint m_locationTextColor;

    public:
        void init();

        void setTextColor(glm::vec3 color) const;
};
