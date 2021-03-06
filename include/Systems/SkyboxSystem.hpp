#pragma once

#include "System.hpp"

class Shader;

// handles drawing and creation of the skybox
class SkyboxSystem : public System {
    private:
        const Shader *m_skyboxShader;

        entt::entity m_player;
        entt::entity m_skybox;

        void _update(int dt) override;

    public:
        SkyboxSystem(Registry_T &registry);
};
