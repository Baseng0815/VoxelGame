#include "System.h"
#include "GBuffer.h"
#include "RenderQuad.h"

#include "Shader.h"

class Event;

struct Light {
    glm::vec3 dir;
    glm::vec3 color;
};

class Shader;
class Texture;

struct CameraComponent;
struct TransformationComponent;

class EntityRenderSystem : public System {
    private:
        // first render pass
        Shader* m_blockShader;
        Shader* m_lightingShader;
        Texture* m_atlas;

        // second render pass
        Light m_light;
        GBuffer m_gBuffer;
        RenderQuad m_renderQuad;

        void handleFramebufferSize(Event* e);

        void _update(int dt) override;

        void updateViewMatrix(CameraComponent& camera, TransformationComponent& transform);
        void updateProjectionMatrix(CameraComponent& camera);

    public:
        EntityRenderSystem(SystemManager* systemManager);
};
