#pragma once

#include "System.h"

class Shader;

// handles drawing and creation of the skybox
class SkyboxSystem : public System {
    private:
        const Shader *m_skyboxShader;

        void _update(int dt) override;

    public:
        SkyboxSystem(Registry_T &registry);
};
