#include "System.h"
#include "GBuffer.h"
#include "TextureAtlas.h"
#include "RenderQuad.h"

#include "BlockShader.h"
#include "LightingShader.h"

class Event;

class EntityRenderSystem : public System {
    private:
        // first render pass
        BlockShader m_blockShader;
        LightingShader m_lightingShader;

        // second render pass
        Light m_light;
        GBuffer m_gBuffer;
        RenderQuad m_renderQuad;

        void handleFramebufferSize(Event* e);

    public:
        EntityRenderSystem(SystemManager* systemManager);

        void update(int dt) override;
};
