#pragma once

#include "System.h"
#include "../Resources/Geometry.h"

class Shader;

// renders various debug info when in debug mode (F3) independently from other rendering systems
class DebugRenderSystem : public System {
    private:
        const Shader *m_mvpColorShader;
        std::vector<Geometry> m_chunkBoundaries;

        bool m_isEnabled = false;

        void _update(int dt) override;

    public:
        DebugRenderSystem(Registry_T &registry);

        void toggle();
};
