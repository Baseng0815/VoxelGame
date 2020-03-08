#include "System.h"
#include "GBuffer.h"
#include "TextureAtlas.h"
#include "RenderQuad.h"

#include "BlockShader.h"
#include "LightingShader.h"

class Event;

class ChunkRenderSystem {
    private:
        // first render pass
        BlockShader m_blockShader;
        LightingShader m_lightingShader;

        // second render pass
        Light m_light;
        GBuffer m_gBuffer;
        RenderQuad m_renderQuad;

        TextureAtlas m_textureAtlas;

        Camera m_camera;

        void handleFramebufferSize(Event* e);

    public:
        ChunkRenderSystem(SystemManager* systemManager);

        void init() override;
        void update(int dt) override;

        ~ChunkRenderSystem();
}
