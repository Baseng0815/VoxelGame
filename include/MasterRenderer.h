#pragma once

#include "World.h"
#include "Skybox.h"
#include "Camera.h"
#include "GBuffer.h"
#include "RenderQuad.h"
#include "TextureAtlas.h"

#include "GUIRenderer.h"

#include "BlockShader.h"
#include "SkyboxShader.h"
#include "LightingShader.h"

class Scene;
class Event;

class MasterRenderer {
    private:
        // first render pass
        BlockShader m_blockShader;
        LightingShader m_lightingShader;

        // second render pass
        Light m_light;
        GBuffer m_gBuffer;
        RenderQuad m_renderQuad;

        // skybox
        SkyboxShader m_skyboxShader;
        Skybox m_skybox;

        TextureAtlas m_textureAtlas;

        GUIRenderer m_guiRenderer;

        void handleFramebufferSize(Event* e);

    public:
        void init();

        void render(const Scene& scene);
};
