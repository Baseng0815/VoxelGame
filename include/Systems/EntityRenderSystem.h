#include "../../include/Systems/System.h"
#include "../../include/Rendering/GBuffer.h"
#include "../../include/Rendering/RenderQuad.h"

#include "../../include/Rendering/Shader.h"

struct Event;

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
        glm::mat4 m_orthoProjection;

        void handleFramebufferSize(Event* e);

        void _update(int dt) override;

    public:
        EntityRenderSystem(entt::registry* registry);
};
