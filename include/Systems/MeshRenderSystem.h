#include "../../include/Systems/System.h"
#include "../../include/Rendering/GBuffer.h"
#include "../../include/Rendering/RenderQuad.h"
#include "../../include/Rendering/Light.h"

class Shader;
class Texture;

struct CameraComponent;
struct TransformationComponent;

struct FramebufferSizeEvent;

// TODO add light components instead of hard coding
#define MAX_LIGHTS 1

class MeshRenderSystem : public System {
    private:
        // TODO use dynamically each frame instead of storing it here
        Shader* m_meshRenderShaderColor;
        Shader* m_meshRenderShaderTexture;
        Texture* m_atlas;

        PointLight m_pointLights[MAX_LIGHTS];
        DirectionalLight m_sun;

        void _update(int dt) override;

    public:
        MeshRenderSystem(entt::registry* registry);
};
