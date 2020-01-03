#pragma once

#include "Light.h"
#include "Shader.h"

class LightingShader : public Shader {
    private:
        GLint m_locationLightPos;
        GLint m_locationLightColor;

    public:
        void init();

        void setLight(Light light, int index);
};
