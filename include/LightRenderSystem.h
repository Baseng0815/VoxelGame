#include "System.h"
#include "GBuffer"
#include "RenderQuad.h"

#include "LightingShader.h"

class Event;

class LightRenderSystem : public System {
    private:
        // second render pass
        Light m_light;
        LightingShader m_lightingShader;
        GBuffer m_gBuffer;
        RenderQuad m_renderQuad;

        void handleFramebufferSize(Event* e);

    public:
        LightRenderSystem(SystemManager* systemManager);

        void update(int dt) override;
};
